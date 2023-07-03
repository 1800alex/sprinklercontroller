#include <stdio.h>
#include <lib/SprinklerHeadController.hpp>

#ifdef PICO
#include "pico/stdlib.h"
#endif

#ifdef LINUX

#include <ncurses.h>
#include <unistd.h>

void displayCycle(SprinklerController::SprinklerHeadController *controller)
{
	move(1, 1);
	printw("Cycle %d", controller->GetCycle() + 1);
}

void displayPumpStatus(SprinklerController::SprinklerHeadController *controller, bool on)
{
	move(2, 1);
	printw("Pump: %s", on ? "ON " : "OFF");
}

void displayHeadStatus(SprinklerController::SprinklerHeadController *controller, uint8_t index,
	bool on)
{
	move(index + 3, 1);
	printw("Head %d: %s", index, on ? "ON " : "OFF");
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

void togglePump(void *c, bool on)
{
	SprinklerController::SprinklerHeadController *controller = static_cast<SprinklerController::SprinklerHeadController *>(c);
	displayPumpStatus(controller, on);
	refresh();
}

void toggleHead(void *c, uint8_t index, bool on)
{
	SprinklerController::SprinklerHeadController *controller = static_cast<SprinklerController::SprinklerHeadController *>(c);
	displayHeadStatus(controller, index, on);
	refresh();
}

void sleepMS(int ms)
{
	usleep(ms * 1000);
}

#endif

int main()
{
#ifdef PICO
	stdio_init_all();
	std::cout << "Running on Pico" << std::endl;
#endif

#ifdef LINUX
	SprinklerController::Options opts = {
		.SleepMSFunction = sleepMS,
		.NumHeads = 8,
		.TogglePumpFunction = togglePump,
		.PumpDelay = 500,
		.ToggleHeadFunction = toggleHead,
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
