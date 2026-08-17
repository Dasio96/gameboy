#ifndef MEMORY_H
#define MEMORY_H

#include "stdint.h"

#define MEMORY_SIZE 0x10000

typedef struct {
  uint8_t ram[MEMORY_SIZE];
} Memory;

void memory_write(Memory *mem, uint16_t address, uint8_t value);
uint8_t memory_read(Memory *mem, uint16_t address);

int memory_load_rom(Memory *mem, const char *filepath);

#endif // !MEMORY
