#include <stdint.h>

#ifndef MEMORY

#define MEMORY
#define MEMORY_SIZE 0x10000

typedef struct {
  uint8_t ram[MEMORY_SIZE];
} Memory;

void memory_write(Memory *mem, uint16_t address, uint8_t value);

uint8_t memory_read(Memory *mem, uint16_t address);

#endif // !MEMORY
