#include <stdio.h>
#include <lib/SprinklerHeadController.hpp>

#ifdef LINUX

#include <unistd.h>
#include "ftxui/component/screen_interactive.hpp"


class SimulatedSprinklerController : public SprinklerHeadController::IController {
public:

	SimulatedSprinklerController();
	~SimulatedSprinklerController();
	void Init(void *controller);
	void SleepMS(void *controller, int ms) override;
	void TogglePump(void *controller, bool state) override;
	void ToggleHead(void *controller, uint8_t index, bool state) override;

private:
	void displayCycle(SprinklerHeadController::Controller *spc);
	void displayPumpStatus(SprinklerHeadController::Controller *spc, bool state);
	void displayHeadStatus(SprinklerHeadController::Controller *spc, uint8_t index,
		bool state);
	void displaySetup(SprinklerHeadController::Controller *controller);
	void displayEnd(void);

	ftxui::ScreenInteractive* screen;
};

#endif
