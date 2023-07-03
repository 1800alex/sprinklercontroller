#ifdef LINUX
#ifndef TESTING
#include <stdio.h>
#include "SimulatedSprinklerController.hpp"
#include <ncurses.h>
#include <unistd.h>

SimulatedSprinklerController::~SimulatedSprinklerController()
{
	displayEnd();
}

void SimulatedSprinklerController::displayCycle(SprinklerHeadController::Controller *spc)
{
	move(1, 1);
	printw("Cycle %d", spc->GetCycle() + 1);
}

void SimulatedSprinklerController::displayPumpStatus(SprinklerHeadController::Controller *spc, bool state)
{
	move(2, 1);
	printw("Pump: %s", state ? "ON " : "OFF");
}

void SimulatedSprinklerController::displayHeadStatus(SprinklerHeadController::Controller *spc, uint8_t index,
	bool state)
{
	move(index + 3, 1);
	printw("Head %d: %s", index, state ? "ON " : "OFF");
}

void SimulatedSprinklerController::displaySetup(SprinklerHeadController::Controller *controller)
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

void SimulatedSprinklerController::displayEnd(void) { endwin(); }

void SimulatedSprinklerController::Init(void *controller)
{
	SprinklerHeadController::Controller *spc = static_cast<SprinklerHeadController::Controller *>(controller);
	displaySetup(spc);
}

void SimulatedSprinklerController::SleepMS(void *controller, int ms)
{
	usleep(ms * 1000);
}

void SimulatedSprinklerController::TogglePump(void *controller, bool state)
{
	SprinklerHeadController::Controller *spc = static_cast<SprinklerHeadController::Controller *>(controller);
	displayPumpStatus(spc, state);
	refresh();

	if(true == state)
	{
		SleepMS(spc, 1000);
	}
	else
	{
		SleepMS(spc, 1000);
	}
}

void SimulatedSprinklerController::ToggleHead(void *controller, uint8_t index, bool state)
{
	SprinklerHeadController::Controller *spc = static_cast<SprinklerHeadController::Controller *>(controller);
	displayHeadStatus(spc, index, state);
	refresh();

	if(true == state)
	{
		SleepMS(spc, 500);
	}
	else
	{
		SleepMS(spc, 200);
	}
}

#endif
#endif
