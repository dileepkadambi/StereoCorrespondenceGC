#pragma once
#include <time.h>
#include <iostream>
//Helper Class for timing information 
//Later use CUDA timing information that can be added to this class

//@Usage 
//Create object start timer using StartTimer()
//Get Elapsed time using GetElapsedTimeInSeconds() method
class Timer 
{
public:
	Timer()
	{
		startClock = 0;
	}

	~Timer(){}

	//Simple timer in Seconds
	void StartTimer()
	{
		startClock = clock();
	}

	float GetElapsedTimeInSeconds()
	{
		if(startClock = 0)
		{
			std::cout << "TIMER NOT STARTED ERROR!!!" << std::endl;
			return 0.0;
		}
		clock_t endClock = clock();
		clock_t totalClocks = endClock - startClock;

		float timeTaken = (float)totalClocks/(float)CLOCKS_PER_SEC;

		return timeTaken;
	}

private:
	clock_t startClock;
	
};