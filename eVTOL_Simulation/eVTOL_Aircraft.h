#pragma once
#include<string>
#include<random>
#include"AircraftResults.h"

const double MSEC_PER_HR = 3600000;

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
	void CheckForFault();
	void Fly();
	void EndFlight();
	bool IsCharging();
	void Charge();
	void DisconnectFromCharger();

	/// <summary>
	/// A quick function for printing the results.
	/// </summary>
	std::string PrintResults();
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
	double FlightTime;
	double NumberOfFlights;
	double TimeCharging;
	double NumberOfTimesCharged;
	double NumberOfFaults;
	double PassengerMiles;

};

