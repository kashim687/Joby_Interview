// eVTOL_Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <deque>
#include <map>
#include "Constants.h"
#include "eVTOL_Aircraft.h"
#include "CompanyResults.h"
#include "AircraftResults.h"

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
	//The ChargingQueue is a deque of pointers that keeps track of who is in line for the chargers.
	std::deque<eVTOL_Aircraft*> ChargingQueue;
	std::vector<eVTOL_Aircraft> Aircraft;
	std::map<std::string, CompanyResults> ResultsByCompany;
	long Simulation_Time_msec = 0;

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

	//Now, actually run the simulation.
	//TODO: This should not be in the main. In a smaller program of this kind, it is easiest to keep it here
	//but if this program were to be expanded, this should be pushed out of the main.
	//TODO: This could be threaded, but that is kind of outside the scope of this exercise.
	while( !(Simulation_Time_msec > SIMULATION_DURATION_SEC * 1000) )
	{
		Simulation_Time_msec += SIMULATION_SPEED_MSEC;
		if( Simulation_Time_msec % MSEC_PER_HR == 0 )
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
			if( (Simulation_Time_msec % MSEC_PER_HR ) == 0 )
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
