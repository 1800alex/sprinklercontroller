#include <stdio.h>
#include <lib/SprinklerHeadController.hpp>

#ifdef PICO
#include "pico/stdlib.h"
#endif

#ifdef LINUX

#include <ncurses.h>
#include <unistd.h>

void displayCycle(SprinklerController::SprinklerHeadController *spc)
{
	move(1, 1);
	printw("Cycle %d", spc->GetCycle() + 1);
}

void displayPumpStatus(SprinklerController::SprinklerHeadController *spc, bool state)
{
	move(2, 1);
	printw("Pump: %s", state ? "ON " : "OFF");
}

void displayHeadStatus(SprinklerController::SprinklerHeadController *spc, uint8_t index,
	bool state)
{
	move(index + 3, 1);
	printw("Head %d: %s", index, state ? "ON " : "OFF");
}

void displaySetup(SprinklerController::SprinklerHeadController *controller)
{
	// Initialize ncurses
	initscr();
	clear();

	move(0, 1);
	printw("Sprinkler controller compiled %s %s", __DATE__, __TIME__);

	// Display initial states
	displayCycle(controller);
	displayPumpStatus(controller, false);
	for(uint8_t i = 0; i < controller->GetNumHeads(); ++i)
	{
		displayHeadStatus(controller, i, false);
	}
	refresh();
}

void displayEnd(SprinklerController::SprinklerHeadController *controller) { endwin(); }

class SimulatedSprinklerController : public SprinklerController::ISprinklerHeadController {
public:
	void SleepMS(void *controller, int ms) override
	{
		usleep(ms * 1000);
	}

	void TogglePump(void *controller, bool state) override
	{
		SprinklerController::SprinklerHeadController *spc = static_cast<SprinklerController::SprinklerHeadController *>(controller);
		displayPumpStatus(spc, state);
		refresh();
	}

	void ToggleHead(void *controller, uint8_t index, bool state) override
	{
		SprinklerController::SprinklerHeadController *spc = static_cast<SprinklerController::SprinklerHeadController *>(controller);
		displayHeadStatus(spc, index, state);
		refresh();
	}
};

#endif

int main()
{
#ifdef PICO
	stdio_init_all();
	std::cout << "Running on Pico" << std::endl;
#endif

#ifdef LINUX
	SimulatedSprinklerController *sim = new SimulatedSprinklerController();
	SprinklerController::Options opts = {
		.ControllerImplementation = sim,
		.NumHeads = 8,
		.PumpDelay = 500,
		.HeadOnTime = 500,
		.HeadOffTime = 200,
	};
	SprinklerController::SprinklerHeadController controller(opts);

	// if(controller == NULL)
	// {
	// 	return -1;
	// }

	displaySetup(&controller);

	while(true)
	{
		displayCycle(&controller);
		controller.Cycle();
	}

	displayEnd(&controller);
	// delete controller;
#endif

	return 0;
}
