#include <stdio.h>
#include <lib/SprinklerHeadController.hpp>

#ifdef PICO
#include "pico/stdlib.h"
#endif

#ifdef LINUX

#include <impl/sim/SimulatedSprinklerController.hpp>

#endif

int main()
{
#ifdef PICO
	stdio_init_all();
	std::cout << "Running on Pico" << std::endl;
#endif

#ifdef LINUX
	SimulatedSprinklerController *sim = new SimulatedSprinklerController();
	SprinklerHeadController::Options opts = {
		.ControllerImplementation = sim,
		.NumHeads = 8,
		.PumpDelay = 1000,
		.HeadOnTime = 500,
		.HeadOffTime = 200,
	};
	SprinklerHeadController::Controller controller(opts);

	// if(controller == NULL)
	// {
	// 	return -1;
	// }

	sim->Init(&controller);

	while(true)
	{
		controller.Cycle();
	}

	// delete controller;
#endif

	return 0;
}
