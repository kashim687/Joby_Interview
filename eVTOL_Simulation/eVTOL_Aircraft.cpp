#pragma once
#include "eVTOL_Aircraft.h"
#include <random>
/// <summary>
/// Constructor for a new Electric Vertical Take Off and Landing Aircraft
/// Pass in the company specs
/// </summary>
eVTOL_Aircraft::eVTOL_Aircraft( std::string companyName, double cruiseSpeed, double batteryCapacity, double timeToCharge,
	double energyUseAtCruise, double passengerCapacity, double faultProbability )
{
	CompanyName = companyName;
	CruiseSpeed_mph = cruiseSpeed;
	BatteryCapacity_kWh = batteryCapacity;
	TimeToCharge_hr = timeToCharge;
	EnergyUseAtCruise_kWhpm = energyUseAtCruise;
	PassengerCapacity = passengerCapacity;
	FaultProbability_pcth = faultProbability;

	Charging = false;

	/// <summary>
	/// Variables for current aircraft state
	/// </summary>
	CurrentBattery_kWh = BatteryCapacity_kWh;
	SimulationSpeed_msec = 1000;

	FlightTime = 0;
	NumberOfFlights = 0;
	TimeCharging = 0;
	NumberOfTimesCharged = 0;
	NumberOfFaults = 0;
	PassengerMiles = 0;

}

/// <summary>
/// Gets and Sets for the aircraft specification variables
/// </summary>
std::string eVTOL_Aircraft::GetCompanyName()
{
	return CompanyName;
}
double eVTOL_Aircraft::GetCruiseSpeed_mph()
{
	return CruiseSpeed_mph;
}
double eVTOL_Aircraft::GetBatteryCapacity_kWh()
{
	return BatteryCapacity_kWh;
}
double eVTOL_Aircraft::GetTimeToCharge_hr()
{
	return TimeToCharge_hr;
}
double eVTOL_Aircraft::GetEnergyUseAtCruise_kWhpm()
{
	return EnergyUseAtCruise_kWhpm;
}
double eVTOL_Aircraft::GetPassengerCapacity()
{
	return PassengerCapacity;
}
double eVTOL_Aircraft::GetFaultProbability()
{
	return FaultProbability_pcth;
}

void eVTOL_Aircraft::SetCompanyName( std::string companyName )
{
	CompanyName = companyName;
}
void eVTOL_Aircraft::SetCruiseSpeed_mph( double cruiseSpeed )
{
	CruiseSpeed_mph = cruiseSpeed;
}
void eVTOL_Aircraft::SetBatteryCapacity_kWh( double batteryCapacity )
{
	BatteryCapacity_kWh = batteryCapacity;
}
void eVTOL_Aircraft::SetTimeToCharge_hr( double timeToCharge )
{
	TimeToCharge_hr = timeToCharge;
}
void eVTOL_Aircraft::SetEnergyUseAtCruise_kWhpm( double energyUseAtCruise )
{
	EnergyUseAtCruise_kWhpm = energyUseAtCruise;
}
void eVTOL_Aircraft::SetPassengerCapacity( double passengerCapacity )
{
	PassengerCapacity = passengerCapacity;
}
void eVTOL_Aircraft::SetFaultProbability_pcth( double faultProbability )
{
	FaultProbability_pcth = faultProbability;
}

/// <summary>
/// Gets the current amount of charge on the battery
/// </summary>
/// <returns></returns>
double eVTOL_Aircraft::GetCurrentBatteryCharge()
{
	return CurrentBattery_kWh;
}
bool eVTOL_Aircraft::IsCharging()
{
	return Charging;
}
void eVTOL_Aircraft::Charge()
{
	Charging = true;
}
void eVTOL_Aircraft::DisconnectFromCharger()
{
	Charging = false;
}


/// <summary>
/// Simulation behaviors
/// </summary>

/// <summary>
/// Discharge the Current Battery by the amount used in this simulation cycle.
/// </summary>
void eVTOL_Aircraft::DischargeBattery()
{
	CurrentBattery_kWh -= ((EnergyUseAtCruise_kWhpm * CruiseSpeed_mph) / MSEC_PER_HR) * SimulationSpeed_msec;
}

/// <summary>
/// Charge the current battery by the amount it can receive in this simulation cycle.
/// </summary>
void eVTOL_Aircraft::ChargeBattery()
{

	CurrentBattery_kWh += (BatteryCapacity_kWh * SimulationSpeed_msec) / (TimeToCharge_hr * MSEC_PER_HR);
	TimeCharging += SimulationSpeed_msec;
	if( CurrentBattery_kWh >= BatteryCapacity_kWh )
	{
		NumberOfTimesCharged += 1;
		CurrentBattery_kWh = BatteryCapacity_kWh;
	}
}

/// <summary>
/// Fly the aircraft for one tick of the simulation
/// </summary>
void eVTOL_Aircraft::Fly()
{
	FlightTime += SimulationSpeed_msec;
	PassengerMiles += (SimulationSpeed_msec / MSEC_PER_HR) * CruiseSpeed_mph * PassengerCapacity;
	DischargeBattery();
}

/// <summary>
/// The aircraft has arrived! Record necessary statistics.
/// </summary>
void eVTOL_Aircraft::EndFlight()
{
	NumberOfFlights += 1;
}

/// <summary>
/// Determine whether or not a fault has occurred
/// Only do this on the hour; Because the stat that we've been given is faults/hour, we cannot
/// subdivide that easily and have the results come out the same, so instead, just check once per hour.
/// </summary>
void eVTOL_Aircraft::CheckForFault()
{
	NumberOfFaults += rand() % 100 < FaultProbability_pcth ? 1 : 0;
}

std::string eVTOL_Aircraft::PrintResults()
{
	std::string results = "";
	results.append( "Company                 : " + CompanyName + "\n" );
	results.append( "Flight Time             : " + std::to_string( FlightTime ) + "\n" );
	results.append( "Number of Flights       : " + std::to_string( NumberOfFlights ) + "\n" );
	results.append( "Time Charging           : " + std::to_string( TimeCharging ) + "\n" );
	results.append( "Number of times Charged : " + std::to_string( NumberOfTimesCharged ) + "\n" );
	results.append( "Number of Faults        : " + std::to_string( NumberOfFaults ) + "\n" );
	results.append( "Passenger Miles         : " + std::to_string( PassengerMiles ) + "\n" );

	results.append( "\n" );
	return results;
}
AircraftResults eVTOL_Aircraft::GetResults()
{
	AircraftResults retval{ 
		FlightTime, 
		NumberOfFlights, 
		TimeCharging, 
		NumberOfTimesCharged, 
		NumberOfFaults, 
		PassengerMiles };
		return retval;
}
