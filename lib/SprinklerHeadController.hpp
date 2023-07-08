#pragma once

#include <cstdint>
#ifdef LINUX
#include <unistd.h>
#endif

namespace SprinklerHeadController {
// Interface for the SprinklerHeadController
class IController {
public:
	virtual void SleepMS(int ms) = 0;
	virtual void TogglePump(bool state) = 0;
	virtual void ToggleHead(int index, bool state) = 0;
};

struct Options
{
	IController *ControllerImplementation;
	int NumHeads;
	int PumpDelay;
	int HeadOnTime;
	int HeadOffTime;
};

class Controller {
public:
	Controller(Options opts);
	~Controller();

	void Cycle();
	int GetHeadState(int index);
	int GetPumpState(void);
	int GetCycle(void);
	int GetNumHeads(void);
	double GetProgress(void);

private:
	int cycle;
	IController *ctrl;
	int numHeads;
	int pumpState;
	int pumpDelay;
	int headOnTime;
	int headOffTime;
	int nextStartIndex;
	int *headState;
	double progress;
};
} // namespace SprinklerHeadController