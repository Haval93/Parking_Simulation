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
	// Create A Simulation Instance
	Simulation_Information firstSimulation;

	// Check For Argument Count
	if (argc > 1)
	{
		Simulation_Information(argc, argv);
	}
	else
	{
		Simulation_Information();
	}



	return 0;
}