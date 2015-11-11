/*
Simulation Source File
Assignment #1
Group Members:
Haval Ahmed
Ryan Stevens
Lauren Peppard
Sami Badra
Geoff Crews
*/

#include "Simulation.h"

// Constructor that Initializes all Values
Simulation_Information::Simulation_Information(int argc, char * argv[])
{
	// Check Argument Count 
	if (argc > 1)
	{
		// Create A File Instance
		std::fstream simulationInput;

		// Open File
		simulationInput.open(argv[1]);

		while (simulationInput.is_open() && simulationInput.good())
		{
			// Number of Parking Spots
			simulationInput >> parkingSpots;
			// Number of cars
			simulationInput >> numberOfCars;
			// Arrival Rate
			simulationInput >> arrivalRate;
			// Interarrival High Rate
			simulationInput >> parkIntervalHigh;
			// Interarrival Low Rate
			simulationInput >> parkIntervalLow;
			// Exit Gate
			simulationInput >> exitGate;
		}
		// Close File 
		simulationInput.close();
	}
	else
	{
		// Default Initialization Variables. Will be Read In Exactly This Order
		parkingSpots = 60;
		numberOfCars = 60;
		arrivalRate = 60;
		parkIntervalHigh = 120;
		parkIntervalLow = 60;
		exitGate = 60;
	}

	// Statistical Counters.
	simulationTime = 0.0;
	nextEventType = 0;
	numberOfCustomersDelayed = 0;
	totalNumberOfCustomers = 0;
	numberInEntranceQueue = 0;
	numberInExitQueue = 0;
	entranceServerStatus = 0;
	exitServerStatus = 0;
	exitServerStatus = IDLE;
	entranceServerStatus = IDLE;
	maxEntranceQueueSize = 0;
	maxExitQueueSize = 0;
	totalEntranceQueueDelayTime = 0.0;
	totalExitQueueDelayTime = 0.0;
	totalSearchTime = 0;
	parked = 0;
	nextLeavingCar = 1.0e+29;
	leavingIndex = 0;
	timeSinceLastEvent = 0.0;
	areaUnderEtranceQueue = 0.0;
	areaUnderExitQueue = 0.0;
	areaEntranceServerStatus = 0.0;
	areaExitServerStatus = 0.0;

	// Make Parking Spot Array The Correct Size
	parkingLotSpots.resize(parkingSpots);

	// Make Each Parking Spot Null or 0. We will add or replace the spot with zero after the spot is taken. 
	for (int i = 0; i < parkingSpots; i++)
		parkingLotSpots[i] = EMPTY;

	// Resize The Vector To The Number Of Cars in the simulation
	arrayOfCars.resize(numberOfCars);

	// Initialization the Cars into the car arrays
	for (int i = 0; i < numberOfCars; i++)
		arrayOfCars[i] = new Car;


	//  Initialize event list.  Since no customers are present, the departure
	// (service completion) event is eliminated from consideration

	// First arrival
	timeOfNextEvent[1] = simulationTime + massDensityFunction();
	// Car Instance For First Class
	Car firstCar = Car();
	firstCar.entranceArrivalTime = timeOfNextEvent[1];
	// Depart entrance queue
	timeOfNextEvent[2] = EMPTY;
	// Leave parking space
	timeOfNextEvent[3] = EMPTY;
	// Arrive at exit queue
	timeOfNextEvent[4] = EMPTY;
	timeOfNextEvent[5] = EMPTY;
}

// Timing Method. Used To Determined Next Event
void Simulation_Information::timing(void)
{
	// MinTimeUntilNextEvent
	float minTimeNextEvent = 1.0e+29;

	// Set to 0 until an event is chosen
	nextEventType = 0;

	// Determine the event type of the next event to occur.
	// Iterate through all event types
	for (int i = 1; i < numberOfEvents; ++i)
	{
		// Find smallest time
		if (timeOfNextEvent[i] < minTimeNextEvent) 
		{
			minTimeNextEvent = timeOfNextEvent[i];
			// Next event is event with smallest time
			nextEventType = i; 
			timeOfLastEvent = simulationTime;
			simulationTime = timeOfNextEvent[i];

		}
	}

	// Check to see whether the event list is empty.
	// If all times of next events are EMPTY no event is scheduled
	if (nextEventType == 0)
	{
		// The event list is empty, so stop the simulation.
		std::cout << "The event list is empty at time: " << simulationTime << std::endl;
		exit(1);
	}

	// The event list is not empty, so advance the simulation clock.
	simulationTime = minTimeNextEvent;
}


// Choose Next Event Type Function 
void Simulation_Information::chooseNextEvent(void)
{
	switch (nextEventType)
	{
	case 1:
		// Arrival at entrance queue
		entranceArrive();
		break;
	case 2:
		// Departure from entrance queue
		entranceDepart();
		break;
	case 3:
		// Leaving lot
		leaveSpot();
		break;
	case 4:
		// Arrival at exit queue
		exitArrive();
		break;
	case 5:
		// Departure from exit queue
		exitDepart();
		break;
	}
}


// Arrival Function For Entrance Gate
void Simulation_Information::entranceArrive(void)
{
	// Schedule next arrival.
	//time_next_event[1] = sim_time + massDensityFunction();
	timeOfNextEvent[1] = simulationTime + massDensityFunction();
	// have to make the next car entracearrivaltime to timeOfNextEvent[1]
	car.entranceArrivalTime = timeOfNextEvent[1];

	if (entranceServerStatus == BUSY) // Check to see whether server is busy.
	{

		//// Check to see whether an overflow condition exists.
		//if (entranceQueue.size() > Q_LIMIT)
		//{
		//	/* The queue has overflowed, so stop the simulation. */
		//	printf("\nOverflow of the array time_arrival at");
		//	printf(" time %f", sim_time);
		//	exit(2);
		//}

		//add car to the queue. havl clean this up depending on how you will add cars
		entranceQueue.push(carOfSome Number);
	}
	else // Server idle
	{
		/* Server is idle, so arriving customer has a delay of zero.  (The
		following two statements are for program clarity and do not affect
		the results of the simulation.) */
		totalEntranceQueueDelayTime = simulationTime - timeOfLastEvent;

		// Increment the number of customers delayed, and make server busy.
		numberOfCustomersDelayed++;
		entranceServerStatus = BUSY;

		// Schedule a departure (service completion).
		timeOfNextEvent[2] = simulationTime + exitGate;
		
		// set the entrance depart time for the car object
		car.entranceDepartTime = timeOfNextEvent[2];
	}
}


// Depart Function For Entrance Gate
void Simulation_Information::entranceDepart(void)
{
	int singleCarSearchTime = 0;
	int lotIndex = 0;

	// Check to see whether the queue is empty.
	if (entranceQueue.size() == 0)
	{
		/* The queue is empty so make the server idle and eliminate the
		departure (service completion) event from consideration. */
		entranceServerStatus = IDLE;
		timeOfNextEvent[2] = EMPTY;
	}
	else
	{
		
		/* Compute the delay of the customer who is beginning service and update
		the total delay accumulator. */
		totalEntranceQueueDelayTime = simulationTime - entranceQueue.carnextinline.entrancearrive;

		/* Increment the number of customers delayed, and schedule departure. */
		numberOfCustomersDelayed++;
		timeOfNextEvent[2] = simulationTime + exitGate;

	}

	// Enter lot and search for spot

	//parked++; // Increment number of cars parked

	// Choose random spot and check to see if it's empty
	do
	{
		singleCarSearchTime += 3;
		totalSearchTime += 3;
		lotIndex = static_cast <int>((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * (parkingSpots - 1));
	} while (parkingLotSpots[lotIndex] <= 1.0e+29); // Try again if spot was taken

										//singleCarSearchTime += parked;

	parked++; // Increment number of cars parked

			  // Store departure time in spot
	parkingLotSpots[lotIndex] = simulationTime + singleCarSearchTime + ParkingTime(parkIntervalLow, parkIntervalHigh);

	// Print departure time of spot
	//printf("Spot %d taken. Departure time: %f seconds. Search time: %d\n", lotIndex, lot[lotIndex], singleCarSearchTime);
	//cout << "Spot " << left << setw(2) << lotIndex << " taken.   Departure time: " << left << setw(8) << lot[lotIndex] << " seconds.   Search time: " << singleCarSearchTime << endl;
	//cout << "Spot number taken: " << left << setw(2) << lotIndex << "   Departure time in seconds: " << left << setw(8) << lot[lotIndex] << "   Search time: " << singleCarSearchTime << endl;

	// Determine the time of the next car to leave a spot
	for (int i = 0; i <= parkingSpots - 1; i++)
	{
		if (parkingLotSpots[i] < nextLeavingCar)
		{
			nextLeavingCar = parkingLotSpots[i];
			leavingIndex = i;
			timeOfNextEvent[3] = parkingLotSpots[i];
		}
	}
	
}

// Function to handle car leaving lot event
void Simulation_Information::leaveSpot(void)
{
	parkingLotSpots[leavingIndex] = EMPTY; // Remove car from spot
	nextLeavingCar = EMPTY;
	parked--; // Decrement number of cars parked

			  // Find car that is leaving next
	for (int i = 0; i <= parkingSpots - 1; ++i)
	{
		if (parkingLotSpots[i] <= nextLeavingCar)
		{
			nextLeavingCar = parkingLotSpots[i];
			leavingIndex = i;
			timeOfNextEvent[3] = parkingLotSpots[i];
		}
	}

	// Car arrives in exit queue
	exitArrive();
}

// Arrival Function For Exit Gate
void Simulation_Information::exitArrive(void)
{

	// Check to see whether server is busy.
	if (exitServerStatus == BUSY)
	{

		/* There is still room in the queue, so store the time of arrival of the
		arriving customer at the (new) end of time_arrival. */

		exitQueue[num_in_q2] = sim_time; /// I dont know what this line is doing
	}

	else
	{
		/* Server is idle, so arriving customer has a delay of zero.  (The
		following two statements are for program clarity and do not affect
		the results of the simulation.) */

		
		totalExitQueueDelayTime += simulationTime - timeOfLastEvent;

		// Increment the number of customers delayed, and make server busy.

		exitServerStatus = BUSY;

		// Schedule a departure (service completion).
		timeOfNextEvent[5] = simulationTime + exitGate;
	}

}


// Depart Function For Exit Gate
void Simulation_Information::exitDepart(void)
{

	// Check to see whether the queue is empty.
	if (exitQueue.size() == 0)
	{
		/* The queue is empty so make the server idle and eliminate the
		departure (service completion) event from consideration. */

		exitServerStatus = IDLE;
		timeOfNextEvent[5] = EMPTY;
	}

	else
	{

		/* Compute the delay of the customer who is beginning service and update
		the total delay accumulator. */

		totalExitQueueDelayTime = simulationTime - exitQueue.car.exitarrivalTime;
		

		// Increment the number of customers delayed, and schedule departure.
		nextEventType[5] = simulationTime + exitGate;

		exitQueue.pop;
	}
}

// Update Average Time Stats
void Simulation_Information::updateAverageTimeStats(void)
{
	/* Compute time since last event, and update last-event-time marker. */

	timeSinceLastEvent = simulationTime - timeOfLastEvent;
	timeOfLastEvent = simulationTime;

	/* Update area under number-in-queue function. */

	areaUnderEtranceQueue += entranceQueue.size() * timeSinceLastEvent;
	areaUnderExitQueue += exitQueue.size() * timeSinceLastEvent;

	/* Update area under server-busy indicator function. */

	areaEntranceServerStatus += entranceServerStatus * timeSinceLastEvent;
	areaExitServerStatus += exitServerStatus * timeSinceLastEvent;
}


// Report Statisical Data
void Simulation_Information::report(void)
{
	
}


float massDensityFunction()
{
	// F(X) = 24 * 3(1 - X)
	// X is a Random Value Between 0-1 Not Including 1
	float randomNumber = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	// If Random Number is 1 Call Method Again
	if (randomNumber == 1.0)
		return massDensityFunction();

	// Return F(X) = 72(1-X). We should be getting from 0-72
	return 72 * (1 - randomNumber);
}

//get a random parking number between a range
float ParkingTime(float parkIntervalLow, float parkIntervalHigh)
{
	//25 + (std::rand() % (63 - 25 + 1))
	float parkTime = 0;
	parkTime = parkIntervalLow - (std::rand() % (parkIntervalHigh - parkIntervalLow + 1));
	return parkTime;
}
