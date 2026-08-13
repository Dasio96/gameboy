#include <stdint.h>
#include <stdio.h>

#define MEMORY_SIZE 0x10000

typedef struct {
  uint8_t ram[MEMORY_SIZE];
} Memory;

void memory_write(Memory *mem, uint16_t address, uint8_t value) {
  mem->ram[address] = value;
}

uint8_t memory_read(Memory *mem, uint16_t address) { return mem->ram[address]; }

int memory_load_rom(Memory *mem, const char *filepath) {
  FILE *file = fopen(filepath, "rb");
  if (file == NULL)
    return 0;

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  if (file_size > MEMORY_SIZE)
    goto cleanup;

  fread(mem->ram, 1, file_size, file);

  int success = 1;

cleanup:
  fclose(file);
  return success;
}
