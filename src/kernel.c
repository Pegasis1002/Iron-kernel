#include "malloc.h"

typedef unsigned int uint32_t;

extern uint32_t heap_start;

static uint32_t current_break = 0;

uint32_t handle_trap(uint32_t cause, uint32_t arg0, uint32_t syscall_id) {
  char* uart = (char*) 0x10000000;

  if (current_break == 0) {
    current_break = (uint32_t)&heap_start;
  }

  if (cause == 8) {
    switch (syscall_id) {
      case 1:
        *uart = (char)arg0;
        break;
      case 4:
        char* str = (char*)arg0;
        while (*str != '\0') {
          *uart = *str;
          str++;
        }
        break;
      case 10:
        uint32_t old_break = current_break;
        if (current_break + arg0 > 0x88000000) {
          return -1;
        }
        current_break += arg0;
        return old_break;
        break;
      case 93:
        *uart = 'Q';
        break;
      default:
        *uart = '?';
    }
  } else {
    *uart = 'X';
  }

  return 0;
}

void test_malloc() {
    char* uart = (char*) 0x10000000;
    
    // 1. Attempt to allocate 64 bytes
    char* buffer = (char*) malloc(64);
    
    if (buffer == (char*)0 || buffer == (char*)-1) {
        // Print 'E' for Error
        *uart = 'E';
        return;
    }

    // 2. Write to the new memory
    buffer[0] = 'M';
    buffer[1] = 'A';
    buffer[2] = 'L';
    buffer[3] = 'L';
    buffer[4] = 'O';
    buffer[5] = 'C';
    buffer[6] = ' ';
    buffer[7] = 'O';
    buffer[8] = 'K';
    buffer[9] = '\n';
    buffer[10] = '\0';

    // 3. Print from the allocated buffer
    char* ptr = buffer;
    while (*ptr != '\0') {
        *uart = *ptr;
        ptr++;
    }
}
