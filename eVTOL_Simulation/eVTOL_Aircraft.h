#pragma once
#include<string>
#include<random>
#include"AircraftResults.h"
#include"Constants.h"

class eVTOL_Aircraft
{

public:
	eVTOL_Aircraft( std::string, double, double, double, double, double, double );

	//Note to reader; C# contains region flags to allow code bits like this to be easily collapsable,
	//but the only thing I could find similar for C++ was #pragma region, which is visual studio specific.
	//I don't feel the need to put comments on gets and sets for function, because they should be self-documenting
	//so I just added REGION: to my summary comments.
	/// <summary>
	/// REGION: Gets and sets for specification variables
	/// </summary>
	std::string GetCompanyName();
	double GetCruiseSpeed_mph();
	double GetBatteryCapacity_kWh();
	double GetTimeToCharge_hr();
	double GetEnergyUseAtCruise_kWhpm();
	double GetPassengerCapacity();
	double GetFaultProbability();

	void SetCompanyName( std::string );
	void SetCruiseSpeed_mph( double );
	void SetBatteryCapacity_kWh( double );
	void SetTimeToCharge_hr( double );
	void SetEnergyUseAtCruise_kWhpm( double );
	void SetPassengerCapacity( double );
	void SetFaultProbability_pcth( double );

	/// <summary>
	/// REGION: Functions to handle various parts of the simulation
	/// </summary>
	double GetCurrentBatteryCharge();
	void DischargeBattery();
	void ChargeBattery();
	/// <summary>
	/// Checks the aircraft for a fault. *Should only be run once per hour*
	/// </summary>
	void CheckForFault();
	/// <summary>
	/// Causes the aircraft to fly for one cycle at its cruise speed
	/// </summary>
	void Fly();
	/// <summary>
	/// Causes the aircraft to land, and triggers end of flight events.
	/// </summary>
	void EndFlight();
	/// <summary>
	/// Tells you whether or not the aircraft is currently charging or in line for a charger.
	/// </summary>
	/// <returns>whether or not the aircraft is currently in line OR charging.</returns>
	bool IsCharging();
	/// <summary>
	/// Actually puts charge into the battery
	/// </summary>
	void Charge();
	/// <summary>
	/// Marks charging false. Could be expanded to reset a charger object.
	/// </summary>
	void DisconnectFromCharger();

	/// <summary>
	/// A quick function for printing the results.
	/// </summary>
	std::string PrintResults();
	/// <summary>
	/// Gets the results from the current aircraft.
	/// </summary>
	AircraftResults GetResults();

private:
	/// <summary>
	/// REGION: Variables for aircraft specifications
	/// </summary>
	std::string CompanyName;
	double CruiseSpeed_mph;
	double BatteryCapacity_kWh;
	double TimeToCharge_hr;
	double EnergyUseAtCruise_kWhpm;
	double PassengerCapacity;
	double FaultProbability_pcth;

	/// <summary>
	/// REGION: Variables for current aircraft state
	/// </summary>
	double CurrentBattery_kWh;
	double SimulationSpeed_msec;
	bool Charging;

	/// <summary>
	/// REGION: Statistics recorded for information purposes.
	/// </summary>
	
	/// <summary>
	/// How long the aircraft has been in the air
	/// </summary>
	double FlightTime;
	/// <summary>
	/// The number of flights *completed*. Does not count flights that did not complete.
	/// </summary>
	double NumberOfFlights;
	/// <summary>
	/// How long the aircraft has spent on the charger.
	/// </summary>
	double TimeCharging;
	/// <summary>
	/// How many times the aircraft has been *fully* charged. Does not count partial charges.
	/// </summary>
	double NumberOfTimesCharged;
	/// <summary>
	/// How many faults have occurred.
	/// </summary>
	double NumberOfFaults;
	/// <summary>
	/// How many miles the aircraft has flown times the number of passengers aboard.
	/// </summary>
	double PassengerMiles;

};

