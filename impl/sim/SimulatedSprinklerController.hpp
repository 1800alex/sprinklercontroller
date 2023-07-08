#include <stdio.h>
#include <lib/SprinklerHeadController.hpp>

#ifdef LINUX

#include <unistd.h>
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

class SimulatedSprinklerController : public SprinklerHeadController::IController {
public:
	SimulatedSprinklerController();
	~SimulatedSprinklerController();
	void Init(SprinklerHeadController::Controller *shc);
	void SleepMS(int ms) override;
	void TogglePump(bool state) override;
	void ToggleHead(int index, bool state) override;

private:
	void displayCycle(void);
	void displayPumpStatus(bool state);
	void displayHeadStatus(int index, bool state);
	void displaySetup(void);
	void displayEnd(void);

	SprinklerHeadController::Controller *controller;

	ftxui::ScreenInteractive *screen;
	ftxui::Component tab_selection;
	ftxui::Component tab_content;
	ftxui::Component main_container;
	ftxui::Component main_renderer;
	int shift;

	std::vector<std::string> tab_entries;
	int tab_index;
};

#endif
