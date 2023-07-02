#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef void (*ToggleFunction)(void *, uint8_t, bool);

typedef struct {
  int cycle;
  uint8_t numHeads;
  ToggleFunction toggleFunction;
  int delayTime;
  int nullTime;
  uint8_t nextStartIndex;
  uint8_t *ledState;
} SprinklerHeadController;

SprinklerHeadController *SprinklerHeadControllerNew(uint8_t numHeads, ToggleFunction toggleFunction,
                                int delayTime, int nullTime);
void SprinklerHeadControllerFree(SprinklerHeadController *controller);
void SprinklerHeadControllerCycle(SprinklerHeadController *controller);
uint8_t SprinklerHeadControllerGetState(SprinklerHeadController *controller, uint8_t index);
