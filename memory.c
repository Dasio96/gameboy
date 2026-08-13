#include <stdint.h>

#define MEMORY_SIZE 0x10000

typedef struct {
  uint8_t ram[MEMORY_SIZE];
} Memory;

void memory_write(Memory *mem, uint16_t address, uint8_t value) {
  mem->ram[address] = value;
}

uint8_t memory_read(Memory *mem, uint16_t address) { return mem->ram[address]; }
