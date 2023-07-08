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
	  headState(new int[opts.NumHeads])
{
	if(headState == nullptr)
	{
		// Handle memory allocation failure
		exit(1);
	}

	pumpState = 0;
	progress = 0;
	memset(headState, 0, sizeof(int) * opts.NumHeads);
}

Controller::~Controller()
{
	delete[] headState;
}

void Controller::Cycle()
{
	this->progress = 0;
	pumpState = 1;
	ctrl->TogglePump(true);
	ctrl->SleepMS(pumpDelay);
	this->progress = 0.01;

	for(int i = 0; i < numHeads; ++i)
	{
		this->progress = 0.01 + (((double)i) / ((double)numHeads));
		int currentLED = (nextStartIndex + i) % numHeads;
		ctrl->ToggleHead(currentLED, true);
		headState[currentLED] = 1;
		ctrl->SleepMS(headOnTime);
		ctrl->ToggleHead(currentLED, false);
		headState[currentLED] = 0;
		ctrl->SleepMS(headOffTime);
	}

	this->progress = 0.99;
	ctrl->TogglePump(false);
	pumpState = 0;
	ctrl->SleepMS(pumpDelay);
	this->progress = 1;

	nextStartIndex = (nextStartIndex + 1) % numHeads;
	cycle++;
}

int Controller::GetHeadState(int index)
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

int Controller::GetPumpState(void)
{
	return pumpState;
}

int Controller::GetNumHeads(void) { return numHeads; }

int Controller::GetCycle(void)
{
	return cycle;
}

double Controller::GetProgress(void)
{
	return progress;
}
} // namespace SprinklerHeadController
