#include "SprinklerHeadController.hpp"
#include <cstdint>
#include <cstdlib>

namespace SprinklerController {
SprinklerHeadController::SprinklerHeadController(Options opts)
	: cycle(0),
	  ctrl(opts.ControllerImplementation),
	  numHeads(opts.NumHeads),
	  pumpDelay(opts.PumpDelay),
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
	ctrl->TogglePump(this, true);
	ctrl->SleepMS(this, pumpDelay);

	for(uint8_t i = 0; i < numHeads; ++i)
	{
		uint8_t currentLED = (nextStartIndex + i) % numHeads;
		ctrl->ToggleHead(this, currentLED, true);
		ledState[currentLED] = 1;
		ctrl->SleepMS(this, headOnTime);
		ctrl->ToggleHead(this, currentLED, false);
		ledState[currentLED] = 0;
		ctrl->SleepMS(this, headOffTime);
	}

	ctrl->TogglePump(this, false);
	ctrl->SleepMS(this, pumpDelay);

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
} // namespace SprinklerController
