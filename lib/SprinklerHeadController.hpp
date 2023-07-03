#pragma once

#include <cstdint>
#ifdef LINUX
#include <unistd.h>
#endif

namespace SprinklerHeadController {
// Interface for the SprinklerHeadController
class IController {
public:
	virtual void TogglePump(void *controller, bool state) = 0;
	virtual void ToggleHead(void *controller, uint8_t index, bool state) = 0;
};

struct Options
{
	IController* ControllerImplementation;
	uint8_t NumHeads;
	uint32_t PumpDelay;
	uint32_t HeadOnTime;
	uint32_t HeadOffTime;
};

class Controller {
public:
	Controller(Options opts);
	~Controller();

	void Cycle();
	uint8_t GetState(uint8_t index);
	int GetCycle(void);
	uint8_t GetNumHeads(void);

private:
	uint32_t cycle;
	IController* ctrl;
	uint8_t numHeads;
	uint32_t pumpDelay;
	uint32_t headOnTime;
	uint32_t headOffTime;
	uint8_t nextStartIndex;
	uint8_t *ledState;
};
} // namespace SprinklerHeadController