#include <gameboy/cpu.h>
#include <gameboy/memory.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("%s\n", argv[0]);
    return 1;
  }

  Memory memory;
  CPU cpu;

  if (!memory_load_rom(&memory, argv[1])) {
    printf("%s\n", argv[1]);
    return 1;
  }

  cpu_init(&cpu, &memory);

  printf("Start\n");

  while (1) {
    cpu_step(&cpu);
  }
  return 0;
}
