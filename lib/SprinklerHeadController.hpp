#pragma once

#include <cstdint>
#ifdef LINUX
#include <unistd.h>
#endif

namespace SprinklerController {
struct Options
{
	void (*SleepMSFunction)(int);
	uint8_t NumHeads;
	void (*TogglePumpFunction)(void *, bool);
	uint32_t PumpDelay;
	void (*ToggleHeadFunction)(void *, uint8_t, bool);
	uint32_t HeadOnTime;
	uint32_t HeadOffTime;
};

class SprinklerHeadController {
public:
	SprinklerHeadController(Options opts);
	~SprinklerHeadController();

	void Cycle();
	uint8_t GetState(uint8_t index);
	int GetCycle(void);
	uint8_t GetNumHeads(void);

private:
	uint32_t cycle;
	void (*sleepMSFunction)(int);
	uint8_t numHeads;
	void (*togglePumpFunction)(void *, bool);
	uint32_t pumpDelay;
	void (*toggleHeadFunction)(void *, uint8_t, bool);
	uint32_t headOnTime;
	uint32_t headOffTime;
	uint8_t nextStartIndex;
	uint8_t *ledState;
};
} // namespace SprinklerController