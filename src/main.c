#include "SprinklerHeadController.h"
#include <stdio.h>

#ifdef PICO
#include "pico/stdlib.h"
#endif

#ifdef LINUX

#include <ncurses.h>

void displayUpdate(SprinklerHeadController *controller, uint8_t index, bool on) {
  // Update the display
  move(1, 1);
  printw("Cycle %d", controller->cycle + 1);

  move(index + 2, 1); // Move to line (index + 1), column 1
  printw("Head %d: %s", index, on ? "ON " : "OFF");
  refresh();
}

void displaySetup(SprinklerHeadController *controller) {
  // Initialize ncurses
  initscr();
  clear();

  // Display initial states
  for (uint8_t i = 0; i < controller->numHeads; ++i) {
    displayUpdate(controller, i, false);
  }
}

void displayEnd(SprinklerHeadController *controller) {
  // End ncurses mode
  endwin();
}

void toggleLED(void *this, uint8_t index, bool on) {
  SprinklerHeadController *controller = (SprinklerHeadController *)this;
  // Your code to toggle an LED goes here.
  // This is just a dummy example.

  displayUpdate(controller, index, on);
  // printf("Toggle LED: %d %s\n", index, (on ? "on" : "off"));
}

#endif

int main() {
#ifdef PICO
  stdio_init_all();
  std::cout << "Running on Pico" << std::endl;
#endif

#ifdef LINUX
  printf("Running on Linux\n");

  SprinklerHeadController *controller = SprinklerHeadControllerNew(3, toggleLED, 1500, 500);

  if (controller == NULL) {
    return -1;
  }

  displaySetup(controller);

  while (true) {
    SprinklerHeadControllerCycle(controller);
    // printf("Cycle completed...\n");
  }

  displayEnd(controller);
  SprinklerHeadControllerFree(controller);
#endif

  return 0;
}
