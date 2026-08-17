#include <gameboy/memory.h>

#include <stdint.h>
#include <stdio.h>

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

  int success = 1;

  if (file_size < 0 || (size_t)file_size > MEMORY_SIZE) {
    success = 0;
    goto cleanup;
  }

  fread(mem->ram, 1, (size_t)file_size, file);

cleanup:
  fclose(file);
  return success;
}
