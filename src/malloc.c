# include "malloc.h"

extern void* sbrk(int increment);

void* malloc(size_t size) {
  size_t aligned_size = (size + 7) & ~7;

  void* ptr = sbrk(aligned_size);

  if (ptr == (void*)-1) {
    return (void*)0; // Out of memory
  }

  return ptr;
}
