#include <stdio.h>
#include <lib/SprinklerHeadController.hpp>

#ifdef PICO
#include "pico/stdlib.h"
#endif

#ifdef LINUX

#include <impl/sim/SimulatedSprinklerController.hpp>
#include <cJSON.h>
#include <stdlib.h>

#endif

int main()
{
#ifdef PICO
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
	const uint LED_PIN = PICO_DEFAULT_LED_PIN;
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	while(true)
	{
		gpio_put(LED_PIN, 1);
		sleep_ms(250);
		gpio_put(LED_PIN, 0);
		sleep_ms(250);
	}
#endif
#endif

#ifdef LINUX
	cJSON *obj = cJSON_CreateObject();

	cJSON_AddBoolToObject(obj, "Test", true);
	char *str = cJSON_Print(obj);
	printf("%s\n",str);
	free(str);

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
	#endif

	// delete controller;

	return 0;
}
