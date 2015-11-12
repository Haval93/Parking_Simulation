/*
Class: CS 558
Project: Parking Lot Project For Assignment #1
Descripition: Parking Lot With One Entrance and One Exit System
Group Members:
Haval Ahmed
Ryan Stevens
Lauren Peppard
Sami Badra
Geoff Crews
*/

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include "Simulation.h"

// Main Function
int main(int argc, char * argv[])
{
	// Random See With Time. Need For Random Variates
	srand(static_cast<unsigned> (time(NULL)));

	// Create A Simulation Instance
	Simulation_Information firstSimulation = Simulation_Information(argc, argv);

	// Simulation Loop. Run Simulation Until Exit Parameter is met.
	while (firstSimulation.numberOfCustomersDelayed < firstSimulation.totalNumberOfCustomers)
	{
		// Timing Function To Determine The Next Event
		firstSimulation.timing();

		// Update Average Stats Ever 10 Customers 
		if (firstSimulation.numberOfCustomersDelayed % 10 == 0)
		{
			// Update time-average statistical accumulators. 
			firstSimulation.updateAverageTimeStats();
		}

		// Switch To The Next Event Type Based on Event Type
		firstSimulation.chooseNextEvent();
	}

	// Call Report Function
	firstSimulation.report();

	// Pause And Wait For Keyboard At The End Of The Program
	system("pause");

	return 0;
}