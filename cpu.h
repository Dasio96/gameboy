#ifndef CPU_H
#define CPU_H

#include "memory.h"
#include <stdint.h>

typedef struct {
  uint8_t a;
  uint8_t b;
  uint8_t c;
  uint8_t d;
  uint8_t e;
  uint8_t f;
  uint8_t h;
  uint8_t l;

  uint16_t sp;
  uint16_t pc;
  Memory *mem;
} CPU;

void cpu_init(CPU *cpu, Memory *mem);
void cpu_step(CPU *cpu);

#endif // !CPU_H
