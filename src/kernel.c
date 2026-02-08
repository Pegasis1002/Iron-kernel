typedef unsigned int uint32_t;

void handle_trap(uint32_t cause, uint32_t arg0, uint32_t syscall_id) {
  char* uart = (char*) 0x10000000;
  if (cause == 8) {
    switch (syscall_id) {
      case 1:
        *uart = (char)arg0;
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

