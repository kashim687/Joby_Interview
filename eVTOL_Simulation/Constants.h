#pragma once

/// <summary>
/// This is a list of useful constants for this program. It gives me a single useful location to store these values.
/// </summary>
const long MSEC_PER_HR = 3600000;
const int NUMBER_OF_AIRCRAFT = 20;
const long SIMULATION_DURATION_SEC = 10800;
const long SIMULATION_SPEED_MSEC = 1000;
const int NUMBER_OF_CHARGING_STATIONS = 3;

/// <summary>
/// A quick enumeration of all of the available companies. New companies can be added here.
/// TODO: All possible vehicles should be read in via a CSS file instead, which would make this enumeration not needed.
/// TODO: Ideally, this would be in its own file called Companies.h, but 
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