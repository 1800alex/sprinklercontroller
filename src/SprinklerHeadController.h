#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef void (*TogglePumpFunction)(void *, bool);
typedef void (*ToggleHeadFunction)(void *, uint8_t, bool);

typedef struct {
  uint8_t NumHeads;
  TogglePumpFunction TogglePumpFunction;
  int PumpDelay;
  ToggleHeadFunction ToggleHeadFunction;
  int HeadOnTime;
  int HeadOffTime;
} SprinklerHeadControllerOptions;

typedef struct {
  int cycle;
  uint8_t numHeads;
  TogglePumpFunction togglePumpFunction;
  int pumpDelay;
  ToggleHeadFunction toggleHeadFunction;
  int headOnTime;
  int headOffTime;
  uint8_t nextStartIndex;
  uint8_t *ledState;
} SprinklerHeadController;

SprinklerHeadController *SprinklerHeadControllerNew(SprinklerHeadControllerOptions opts);
void SprinklerHeadControllerFree(SprinklerHeadController *controller);
void SprinklerHeadControllerCycle(SprinklerHeadController *controller);
uint8_t SprinklerHeadControllerGetState(SprinklerHeadController *controller, uint8_t index);
