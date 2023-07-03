#include "SprinklerHeadController.hpp"
#include <cstdint>
#include <cstdlib>

namespace SprinklerHeadController {
Controller::Controller(Options opts)
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

Controller::~Controller()
{
	delete[] ledState;
}

void Controller::Cycle()
{
	ctrl->TogglePump(this, true);

	for(uint8_t i = 0; i < numHeads; ++i)
	{
		uint8_t currentLED = (nextStartIndex + i) % numHeads;
		ctrl->ToggleHead(this, currentLED, true);
		ledState[currentLED] = 1;
		ctrl->ToggleHead(this, currentLED, false);
		ledState[currentLED] = 0;
	}

	ctrl->TogglePump(this, false);

	nextStartIndex = (nextStartIndex + 1) % numHeads;
	cycle++;
}

uint8_t Controller::GetState(uint8_t index)
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

uint8_t Controller::GetNumHeads(void) { return numHeads; }

int Controller::GetCycle(void)
{
	return cycle;
}
} // namespace SprinklerHeadController
