#include <stdio.h>
#include <lib/SprinklerHeadController.hpp>

#ifdef LINUX

#include <ncurses.h>
#include <unistd.h>

class SimulatedSprinklerController : public SprinklerHeadController::IController {
public:

	~SimulatedSprinklerController();
	void Init(void *controller);
	void TogglePump(void *controller, bool state) override;
	void ToggleHead(void *controller, uint8_t index, bool state) override;

private:
	void displayCycle(SprinklerHeadController::Controller *spc);
	void displayPumpStatus(SprinklerHeadController::Controller *spc, bool state);
	void displayHeadStatus(SprinklerHeadController::Controller *spc, uint8_t index,
		bool state);
	void displaySetup(SprinklerHeadController::Controller *controller);
	void displayEnd(void);
	void SleepMS(void *controller, int ms);
};

#endif
