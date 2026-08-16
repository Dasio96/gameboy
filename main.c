#include "cpu.h"
#include "memory.h"
#include <stdio.h>

int main() {
  Memory memory;
  CPU cpu;

  cpu_init(&cpu, &memory);

  printf("Start\n");

  while (1) {
    cpu_step(&cpu);
  }
  return 0;
}
