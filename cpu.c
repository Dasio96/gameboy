#include "cpu.h"
#include "memory.h"
#include <stdint.h>

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10

void cpu_init(CPU *cpu, Memory *memory) {
  cpu->pc = 0x0100;
  cpu->sp = 0xFFFE;

  cpu->a = 0;
  cpu->b = 0;
  cpu->c = 0;
  cpu->d = 0;
  cpu->e = 0;
  cpu->f = 0;
  cpu->h = 0;
  cpu->l = 0;

  cpu->mem = memory;
}

void cpu_step(CPU *cpu) {
  uint8_t opcode = memory_read(cpu->mem, cpu->pc);
  cpu->pc++;

  switch (opcode) {
  case 0x00:
    break;
  case 0xFF:
    break;
  case 0x06: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc);
    cpu->pc++;
    cpu->a = arg;
    break;
  }
  case 0xC6: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc);
    cpu->pc++;
    cpu->a += arg;
    break;
  }
  case 0xD6: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc);
    cpu->pc++;
    cpu->a -= arg;
    break;
  }
  case 0xE6: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc);
    cpu->pc++;
    cpu->a &= arg;
    break;
  }
  case 0xF6: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc);
    cpu->pc++;
    cpu->a |= arg;
    break;
  }
  case 0xEE: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc);
    cpu->pc++;
    cpu->a ^= arg;
    break;
  }
  case 0xFE: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc);
    cpu->pc++;
    uint8_t val = cpu->a - arg;

    if (val == 0)
      cpu->f |= FLAG_Z;
    else
      cpu->f &= ~FLAG_Z;

    if (cpu->a < arg)
      cpu->f |= FLAG_C;
    else
      cpu->f &= ~FLAG_C;

    cpu->f |= FLAG_N;

    if ((cpu->a & 0x0F) < (arg & 0x0F))
      cpu->f |= FLAG_H;
    else
      cpu->f &= ~FLAG_H;
    break;
  }
  case 0x0C:
    cpu->a++;

    if (cpu->a == 0)
      cpu->f |= FLAG_Z;
    else
      cpu->f &= ~FLAG_Z;

    cpu->f &= ~FLAG_N;

    if ((cpu->a & 0x0F) == 0)
      cpu->f |= FLAG_H;
    else
      cpu->f &= ~FLAG_H;
    break;
  default:
    break;
  }
}
