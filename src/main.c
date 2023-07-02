#include "SprinklerHeadController.h"
#include <stdio.h>

#ifdef PICO
#include "pico/stdlib.h"
#endif

#ifdef LINUX

#include <ncurses.h>

void displayCycle(SprinklerHeadController *controller) {
  move(1, 1);
  printw("Cycle %d", controller->cycle + 1);
}

void displayUpdatePump(SprinklerHeadController *controller, bool on) {
  move(2, 1);
  printw("Pump: %s", on ? "ON " : "OFF");
}

void displayUpdate(SprinklerHeadController *controller, uint8_t index,
                   bool on) {
  displayCycle(controller);

  move(index + 3, 1); // Move to line (index + 1), column 1
  printw("Head %d: %s", index, on ? "ON " : "OFF");
}

void displaySetup(SprinklerHeadController *controller) {
  // Initialize ncurses
  initscr();
  clear();

  // Display initial states
  displayCycle(controller);
  displayUpdatePump(controller, false);
  for (uint8_t i = 0; i < controller->numHeads; ++i) {
    displayUpdate(controller, i, false);
  }
  refresh();
}

void displayEnd(SprinklerHeadController *controller) {
  // End ncurses mode
  endwin();
}

void togglePump(void *this, bool on) {
  SprinklerHeadController *controller = (SprinklerHeadController *)this;
  displayUpdatePump(controller, on);
  refresh();
}

void toggleHead(void *this, uint8_t index, bool on) {
  SprinklerHeadController *controller = (SprinklerHeadController *)this;
  displayUpdate(controller, index, on);
  refresh();
}

#endif

int main() {
#ifdef PICO
  stdio_init_all();
  std::cout << "Running on Pico" << std::endl;
#endif

#ifdef LINUX
  printf("Running on Linux\n");

  SprinklerHeadControllerOptions opts = {.NumHeads = 3,
                                         .HeadOnTime = 1500,
                                         .HeadOffTime = 500,
                                         .ToggleHeadFunction = toggleHead,
                                         .TogglePumpFunction = togglePump,
                                         .PumpDelay = 5000};
  SprinklerHeadController *controller = SprinklerHeadControllerNew(opts);

  if (controller == NULL) {
    return -1;
  }

  displaySetup(controller);

  while (true) {
    displayCycle(controller);
    SprinklerHeadControllerCycle(controller);
  }

  displayEnd(controller);
  SprinklerHeadControllerFree(controller);
#endif

  return 0;
}
