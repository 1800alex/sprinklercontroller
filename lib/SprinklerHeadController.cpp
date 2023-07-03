#include "SprinklerHeadController.hpp"
#include <cstdint>
#include <cstdlib>

namespace SprinklerController {
SprinklerHeadController::SprinklerHeadController(Options opts)
	: cycle(0),
	  sleepMSFunction(opts.SleepMSFunction),
	  numHeads(opts.NumHeads),
	  togglePumpFunction(opts.TogglePumpFunction),
	  pumpDelay(opts.PumpDelay),
	  toggleHeadFunction(opts.ToggleHeadFunction),
	  headOnTime(opts.HeadOnTime),
	  headOffTime(opts.HeadOffTime),
	  nextStartIndex(0),
	  ledState(new uint8_t[opts.NumHeads])
{
	if(ledState == nullptr)
	{
		// Handle memory allocation failure
		exit(1);
	}
}

SprinklerHeadController::~SprinklerHeadController()
{
	delete[] ledState;
}

void SprinklerHeadController::Cycle()
{
	if(togglePumpFunction != nullptr)
	{
		togglePumpFunction(this, true);
		sleepMSFunction(pumpDelay);
	}

	for(uint8_t i = 0; i < numHeads; ++i)
	{
		uint8_t currentLED = (nextStartIndex + i) % numHeads;
		toggleHeadFunction(this, currentLED, true);
		ledState[currentLED] = 1;
		sleepMSFunction(headOnTime);
		toggleHeadFunction(this, currentLED, false);
		ledState[currentLED] = 0;
		sleepMSFunction(headOffTime);
	}

	if(togglePumpFunction != nullptr)
	{
		togglePumpFunction(this, false);
		sleepMSFunction(pumpDelay);
	}

	nextStartIndex = (nextStartIndex + 1) % numHeads;
	cycle++;
}

uint8_t SprinklerHeadController::GetState(uint8_t index)
{
	if(index < numHeads)
	{
		return ledState[index];
	}
	else
	{
		return -1;
	}
}

uint8_t SprinklerHeadController::GetNumHeads(void) { return numHeads; }

int SprinklerHeadController::GetCycle(void)
{
	return cycle;
}
}
