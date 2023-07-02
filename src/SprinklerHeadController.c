// SprinklerHeadController.c
#include "SprinklerHeadController.h"
#include <stdlib.h>
#ifdef LINUX
#include <unistd.h>
#endif

SprinklerHeadController *SprinklerHeadControllerNew(uint8_t numHeads, ToggleFunction toggleFunction,
                                int delayTime, int nullTime) {
  SprinklerHeadController *controller = malloc(sizeof(SprinklerHeadController));

  if (controller == NULL) {
    return NULL;
  }

  controller->cycle = 0;
  controller->numHeads = numHeads;
  controller->toggleFunction = toggleFunction;
  controller->delayTime = delayTime;
  controller->nullTime = nullTime;
  controller->nextStartIndex = 0;
  controller->ledState = calloc(numHeads, sizeof(uint8_t));

  if (controller->ledState == NULL) {
    free(controller);
    return NULL;
  }

  return controller;
}

void SprinklerHeadControllerFree(SprinklerHeadController *controller) {
  free(controller->ledState);
  free(controller);
}

void SprinklerHeadControllerCycle(SprinklerHeadController *controller) {
  for (uint8_t i = 0; i < controller->numHeads; ++i) {
    uint8_t currentLED = (controller->nextStartIndex + i) % controller->numHeads;
    controller->toggleFunction((void *)controller, currentLED, true);
    controller->ledState[currentLED] = 1;
#ifdef LINUX
    usleep(controller->delayTime * 1000);
#endif
    controller->toggleFunction((void *)controller, currentLED, false);
    controller->ledState[currentLED] = 0;
#ifdef LINUX
    usleep(controller->nullTime * 1000);
#endif
  }

  controller->nextStartIndex =
      (controller->nextStartIndex + 1) % controller->numHeads;
  controller->cycle++;
}

uint8_t SprinklerHeadControllerGetState(SprinklerHeadController *controller, uint8_t index) {
  if (index < controller->numHeads) {
    return controller->ledState[index];
  } else {
    return -1;
  }
}
