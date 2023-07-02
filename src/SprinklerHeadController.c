// SprinklerHeadController.c
#include "SprinklerHeadController.h"
#include <stdlib.h>
#ifdef LINUX
#include <unistd.h>
#endif

SprinklerHeadController *SprinklerHeadControllerNew(SprinklerHeadControllerOptions opts)
{
	SprinklerHeadController *controller = malloc(sizeof(SprinklerHeadController));

	if(controller == NULL)
	{
		return NULL;
	}

	controller->cycle = 0;
	controller->numHeads = opts.NumHeads;
	controller->togglePumpFunction = opts.TogglePumpFunction;
	controller->pumpDelay = opts.PumpDelay;
	controller->toggleHeadFunction = opts.ToggleHeadFunction;
	controller->headOnTime = opts.HeadOnTime;
	controller->headOffTime = opts.HeadOffTime;
	controller->nextStartIndex = 0;
	controller->ledState = calloc(opts.NumHeads, sizeof(uint8_t));

	if(controller->ledState == NULL)
	{
		free(controller);
		return NULL;
	}

	return controller;
}

void SprinklerHeadControllerFree(SprinklerHeadController *controller)
{
	free(controller->ledState);
	free(controller);
}

void SprinklerHeadControllerCycle(SprinklerHeadController *controller)
{
	if(NULL != controller->togglePumpFunction)
	{
		controller->togglePumpFunction((void *)controller, true);
#ifdef LINUX
		usleep(controller->pumpDelay * 1000);
#endif
	}
	for(uint8_t i = 0; i < controller->numHeads; ++i)
	{
		uint8_t currentLED = (controller->nextStartIndex + i) % controller->numHeads;
		controller->toggleHeadFunction((void *)controller, currentLED, true);
		controller->ledState[currentLED] = 1;
#ifdef LINUX
		usleep(controller->headOnTime * 1000);
#endif
		controller->toggleHeadFunction((void *)controller, currentLED, false);
		controller->ledState[currentLED] = 0;
#ifdef LINUX
		usleep(controller->headOffTime * 1000);
#endif
	}

	if(NULL != controller->togglePumpFunction)
	{
		controller->togglePumpFunction((void *)controller, false);
#ifdef LINUX
		usleep(controller->pumpDelay * 1000);
#endif
	}

	controller->nextStartIndex =
		(controller->nextStartIndex + 1) % controller->numHeads;
	controller->cycle++;
}

uint8_t SprinklerHeadControllerGetState(SprinklerHeadController *controller, uint8_t index)
{
	if(index < controller->numHeads)
	{
		return controller->ledState[index];
	}
	else
	{
		return -1;
	}
}
