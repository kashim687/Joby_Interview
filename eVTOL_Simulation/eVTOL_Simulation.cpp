// eVTOL_Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <deque>
#include <map>
#include "eVTOL_Aircraft.h"
#include "CompanyResults.h"
#include "AircraftResults.h"
/// <summary>
/// A quick enumeration of all of the available companies. New companies can be added here.
/// TODO: All possible vehicles should be read in via a CSS file instead, which would make this enumeration not needed.
/// </summary>
enum Companies
{
	Alpha = 0,
	Bravo,
	Charlie,
	Delta,
	Echo,
	END
};

/// <summary>
/// Creates a new aircraft and returns it
/// </summary>
/// <param name="company">The company that makes the craft</param>
/// <returns>the new aircraft</returns>
eVTOL_Aircraft* CreateCraft( Companies company )
{
	//TODO: put stats into a CSS file to be read in at start instead.
	switch( company )
	{
	case Alpha:
		return new eVTOL_Aircraft( "Alpha Company", 120, 320, 0.6, 1.6, 4, 0.25 );
		break;
	case Bravo:
		return new eVTOL_Aircraft( "Bravo Company", 100, 100, 0.2, 1.5, 5, 10 );
		break;
	case Charlie:
		return new eVTOL_Aircraft( "Charlie Company", 160, 220, 0.8, 2.2, 3, 5 );
		break;
	case Delta:
		return new eVTOL_Aircraft( "Delta Company", 90, 120, 0.62, 0.8, 2, 22 );
		break;
	case Echo:
		return new eVTOL_Aircraft( "Echo Company", 30, 150, 0.3, 5.8, 2, 61 );
		break;
	default:
		return new eVTOL_Aircraft( "ERROR", 0, 0, 0, 0, 0, 0 );
		break;
	}
}

int main()
{
	//First, put all of the raw numbers in.
	const int NUMBER_OF_AIRCRAFT = 20;
	const long SIMULATION_DURATION_SEC = 10800;
	const long SIMULATION_SPEED_MSEC = 1000;
	const int NUMBER_OF_CHARGING_STATIONS = 20;
	std::vector<eVTOL_Aircraft> Aircraft;
	std::map<std::string, CompanyResults> ResultsByCompany;

	std::string CompanyNames[] = {"Alpha Company", "Bravo Company", "Charlie Company", "Delta Company", "Echo Company"};
	//Note: These craft are created here because it seems largely preferable that we have at least one of each
	//craft in the simulation during all runs. So we are only going to randomize the others.
	for( int i = 0; i < Companies::END; ++i )
	{
		Aircraft.push_back( *CreateCraft( Companies( i ) ) );
		CompanyResults theseResults = { CompanyNames[i], 0,0,0,0,0,0,0};
		ResultsByCompany.insert( std::pair< std::string, CompanyResults>( CompanyNames[i], theseResults ) );
	}

	//Create the other aircraft at random
	for( int i = 0; i < NUMBER_OF_AIRCRAFT - int(Companies::END); ++i )
	{
		Aircraft.push_back( *CreateCraft( Companies( rand() % int( Companies::END ) ) ) );
	}
	//Create the charging queue. Using a deque because it is possible that people could want to jump priority in some later version
	//Also, this needs to be a queue of pointers because we want to not create additional objects by accident.
	std::deque<eVTOL_Aircraft*> ChargingQueue;
	//Now, actually run the simulation.
	long Simulation_Time_msec = 0;
	while( !(Simulation_Time_msec > SIMULATION_DURATION_SEC * 1000) )
	{
		Simulation_Time_msec += SIMULATION_SPEED_MSEC;
		if( Simulation_Time_msec % 3600000 == 0 )
		{
			std::cout << "An Hour has passed" << std::endl;
		}
		//Start by charging vehicles. Vehicles that are done charging are removed from the charging queue.
		//charging is limited to NUMBER_OF_CHARGERS.
		int i = 0;
		for( std::deque<eVTOL_Aircraft*>::iterator it = ChargingQueue.begin(); it != ChargingQueue.end() && i < NUMBER_OF_CHARGING_STATIONS; ++i )
		{
			(*it)->ChargeBattery();
			//Check to see if it is done charging
			if( (*it)->GetCurrentBatteryCharge() >= (*it)->GetBatteryCapacity_kWh())
			{
				//done charging
				(*it)->DisconnectFromCharger();
				it = ChargingQueue.erase( it );

			}
			else
			{
				//still charging
				++it;
			}
		}
		//Iterate over the vector of aircraft
		//Aircraft that finished charging this round WILL fly this round ALSO. However, with the simulation speed
		//set to a low number, this should be inconsequential.
		for( std::vector<eVTOL_Aircraft>::iterator it = Aircraft.begin(); it != Aircraft.end(); ++it )
		{
			if( (Simulation_Time_msec % 3600000) == 0 )
			{
				it->CheckForFault();
			}
			if( !it->IsCharging() )
			{
				it->Fly();
			}

			if( it->GetCurrentBatteryCharge() <= 0 && !it->IsCharging() )
			{
				it->EndFlight();
				it->Charge();

				ChargingQueue.push_back( &(*it) );
			}
		}
	}
	//Print the simulation results
	std::cout << "Results by aircraft: " << std::endl << std::endl;
	for( int i = 0; i < Aircraft.size(); ++i )
	{
		//TODO: print to a CSS file
		std::cout << Aircraft[i].PrintResults();
	}

	//Print results by company
	std::cout << "Results by company: " << std::endl << std::endl;
	
	for( int i = 0; i < Aircraft.size(); ++i )
	{
		//TODO: print to a CSS file
		std::string thisCompany = Aircraft[i].GetCompanyName();
		AircraftResults theseResults = Aircraft[i].GetResults();
		ResultsByCompany[thisCompany].NumberOfAircraft++;
		ResultsByCompany[thisCompany].TotalFlightTime += theseResults.FlightTime;
		ResultsByCompany[thisCompany].NumberOfFaults += theseResults.NumberOfFaults;
		ResultsByCompany[thisCompany].NumberOfFlights += theseResults.NumberOfFlights;
		ResultsByCompany[thisCompany].NumberOfTimesCharged += theseResults.NumberOfTimesCharged;
		ResultsByCompany[thisCompany].PassengerMiles += theseResults.PassengerMiles;
		ResultsByCompany[thisCompany].TotalTimeCharging += theseResults.TimeCharging;
	}
	for( auto it = ResultsByCompany.begin(); it != ResultsByCompany.end(); ++it )
	{
		//First, raw data, then averages per aircraft
		std::cout << "Results for " << it->second.CompanyName << std::endl;
		std::cout << std::endl;
		std::cout << "Number of Aircraft Simulated " << it->second.NumberOfAircraft << std::endl;
		std::cout << "Total Flight Time (hours)    " << it->second.TotalFlightTime / MSEC_PER_HR << std::endl;
		std::cout << "Number of Faults             " << it->second.NumberOfFaults << std::endl;
		std::cout << "Number of Flights Completed  " << it->second.NumberOfFlights << std::endl;
		std::cout << "Number of Charges Completed  " << it->second.NumberOfTimesCharged << std::endl;
		std::cout << "Passenger Miles              " << it->second.PassengerMiles << std::endl;
		std::cout << "Total Charging Time (hours)  " << it->second.TotalTimeCharging / MSEC_PER_HR << std::endl;
		std::cout << std::endl;
		std::cout << "Average Flight Time (hours)  " << it->second.TotalFlightTime / MSEC_PER_HR / it->second.NumberOfAircraft << std::endl;
		std::cout << "Average Faults               " << it->second.NumberOfFaults / it->second.NumberOfAircraft << std::endl;
		std::cout << "Average Flights Completed    " << it->second.NumberOfFlights / it->second.NumberOfAircraft << std::endl;
		std::cout << "Average Charges Completed    " << it->second.NumberOfTimesCharged / it->second.NumberOfAircraft << std::endl;
		std::cout << "Average Passenger Miles      " << it->second.PassengerMiles / it->second.NumberOfAircraft << std::endl;
		std::cout << "Average Charging Time (hours)" << it->second.TotalTimeCharging / MSEC_PER_HR / it->second.NumberOfAircraft << std::endl;
		std::cout << std::endl;
	}

	return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
