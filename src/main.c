#include <stdio.h>
#include "SprinklerHeadController.h"

#ifdef PICO
#include "pico/stdlib.h"
#endif

#ifdef LINUX

#include <ncurses.h>

void displayCycle(SprinklerHeadController *controller)
{
	move(1, 1);
	printw("Cycle %d", controller->cycle + 1);
}

void displayPumpStatus(SprinklerHeadController *controller, bool on)
{
	move(2, 1);
	printw("Pump: %s", on ? "ON " : "OFF");
}

void displayHeadStatus(SprinklerHeadController *controller, uint8_t index,
	bool on)
{
	move(index + 3, 1); // Move to line (index + 1), column 1
	printw("Head %d: %s", index, on ? "ON " : "OFF");
}

void displaySetup(SprinklerHeadController *controller)
{
	// Initialize ncurses
	initscr();
	clear();

	move(0, 1);
	printw("Sprinkler controller compiled %s %s", __DATE__, __TIME__);

	// Display initial states
	displayCycle(controller);
	displayPumpStatus(controller, false);
	for(uint8_t i = 0; i < controller->numHeads; ++i)
	{
		displayHeadStatus(controller, i, false);
	}
	refresh();
}

void displayEnd(SprinklerHeadController *controller) { endwin(); }

void togglePump(void *this, bool on)
{
	SprinklerHeadController *controller = (SprinklerHeadController *)this;
	displayPumpStatus(controller, on);
	refresh();
}

void toggleHead(void *this, uint8_t index, bool on)
{
	SprinklerHeadController *controller = (SprinklerHeadController *)this;
	displayHeadStatus(controller, index, on);
	refresh();
}

#endif

int main()
{
#ifdef PICO
	stdio_init_all();
	std::cout << "Running on Pico" << std::endl;
#endif

#ifdef LINUX
	SprinklerHeadControllerOptions opts = {.NumHeads = 3,
		.HeadOnTime = 1500,
		.HeadOffTime = 500,
		.ToggleHeadFunction = toggleHead,
		.TogglePumpFunction = togglePump,
		.PumpDelay = 500};
	SprinklerHeadController *controller = SprinklerHeadControllerNew(opts);

	if(controller == NULL)
	{
		return -1;
	}

	displaySetup(controller);

	while(true)
	{
		displayCycle(controller);
		SprinklerHeadControllerCycle(controller);
	}

	displayEnd(controller);
	SprinklerHeadControllerFree(controller);
#endif

	return 0;
}
