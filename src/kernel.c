#include "malloc.h"

typedef unsigned int uint32_t;

extern uint32_t heap_start;

static uint32_t current_break = 0;

uint32_t handle_trap(uint32_t cause, uint32_t arg0, uint32_t syscall_id) {
  char* uart = (char*) 0x10000000;

  if (current_break == 0) {
    current_break = (uint32_t)&heap_start;
  }

  if (cause & 0x80000000) {
    uint32_t code = cause & 0x7FFFFFFF;
    if (code == 7) { // Machine Timer Interrupt
      *uart = '.'; // Print a dot to show the heartbeat!

      // Set next alarm for 100ms later
      volatile uint32_t* mtime_low = (uint32_t*) 0x0200BFF8;
      volatile uint32_t* mtimecmp_low = (uint32_t*) 0x02004000;
      *mtimecmp_low = (*mtime_low) + 100;
    }
  } else {
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

void test_timer() {
  char* uart = (char*) 0x10000000;

  volatile uint32_t* mtime_low = (uint32_t*) 0x0200BFF8;
  uint32_t start_time = *mtime_low;

  for (int i = 0; i < 100000; i++) {
    asm volatile("nop");
  }

  uint32_t end_time = *mtime_low;

  if (end_time > start_time) {
    *uart = 'T';
  } else {
    *uart = 'F';
  }
}

void setup_timer_alarm() {
    volatile uint32_t* mtime_low = (uint32_t*) 0x0200BFF8;
    volatile uint32_t* mtimecmp_low = (uint32_t*) 0x02004000;

    // Set the alarm to 1000ms (1 second) in the future
    uint32_t now = *mtime_low;
    *mtimecmp_low = now + 1000; 
}

void test_timer_echo() {
    volatile uint32_t* mtime_low = (uint32_t*) 0x0200BFF8;
    char* uart = (char*) 0x10000000;

    uint32_t t1 = *mtime_low;
    
    // Busy loop to let host time pass
    for (int i = 0; i < 500000; i++) { asm volatile("nop"); }
    
    uint32_t t2 = *mtime_low;

    if (t2 > t1) {
        *uart = 'T'; // Success: Timer is Ticking!
    } else {
        *uart = 'F'; // Failure: Timer is Frozen at 0
    }
}
