#include "shell.h"

typedef unsigned int uint32_t;

void run_shell() {
  char* uart = (char*) 0x10000000;
  volatile uint32_t* keyboard = (uint32_t*) 0x10000060;

  while (1) {
    uint32_t key = *keyboard;

    if (key != 0) {
      *uart = (char)key;
      *keyboard = 0;
    }
  }
}
