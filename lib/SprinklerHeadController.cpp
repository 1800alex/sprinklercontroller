#include "SprinklerHeadController.hpp"
#include <cstdint>
#include <cstdlib>
#include <memory.h>

namespace SprinklerHeadController {
Controller::Controller(Options opts)
	: cycle(0),
	  ctrl(opts.ControllerImplementation),
	  numHeads(opts.NumHeads),
	  pumpDelay(opts.PumpDelay),
	  headOnTime(opts.HeadOnTime),
	  headOffTime(opts.HeadOffTime),
	  nextStartIndex(0),
	  headState(new uint8_t[opts.NumHeads])
{
	if(headState == nullptr)
	{
		// Handle memory allocation failure
		exit(1);
	}

	pumpState = 0;
	memset(headState, 0, sizeof(uint8_t) * opts.NumHeads);
}

Controller::~Controller()
{
	delete[] headState;
}

void Controller::Cycle()
{
	pumpState = 1;
	ctrl->TogglePump(this, true);
	ctrl->SleepMS(this, pumpDelay);

	for(uint8_t i = 0; i < numHeads; ++i)
	{
		uint8_t currentLED = (nextStartIndex + i) % numHeads;
		ctrl->ToggleHead(this, currentLED, true);
		headState[currentLED] = 1;
		ctrl->SleepMS(this, headOnTime);
		ctrl->ToggleHead(this, currentLED, false);
		headState[currentLED] = 0;
		ctrl->SleepMS(this, headOffTime);
	}

	ctrl->TogglePump(this, false);
	pumpState = 0;
	ctrl->SleepMS(this, pumpDelay);

	nextStartIndex = (nextStartIndex + 1) % numHeads;
	cycle++;
}

uint8_t Controller::GetHeadState(uint8_t index)
{
	if(index < numHeads)
	{
		return headState[index];
	}
	else
	{
		return -1;
	}
}

uint8_t Controller::GetPumpState(void)
{
	return pumpState;
}

uint8_t Controller::GetNumHeads(void) { return numHeads; }

int Controller::GetCycle(void)
{
	return cycle;
}
} // namespace SprinklerHeadController
