#include "cpu.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10

void cpu_init(CPU *cpu, Memory *memory) {
  cpu->pc = 0x0100;
  cpu->sp = 0xFFFE;

  cpu->a = 0x01;
  cpu->b = 0x00;
  cpu->c = 0x00;
  cpu->d = 0x00;
  cpu->e = 0x00;
  cpu->f = 0xB0;
  cpu->h = 0x00;
  cpu->l = 0x00;

  cpu->mem = memory;
}

void cpu_step(CPU *cpu) {
  uint8_t opcode = memory_read(cpu->mem, cpu->pc);
  cpu->pc++;

  switch (opcode) {
  case 0x00:
    break;

  case 0x01: {
    uint16_t val = memory_read(cpu->mem, cpu->pc++) |
                   (memory_read(cpu->mem, cpu->pc++) << 8);
    cpu->c = val & 0xFF;
    cpu->b = (val >> 8) & 0xFF;
    break;
  }

  case 0x06: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->b = arg;
    break;
  }

  case 0x0C: {
    if ((cpu->c & 0x0F) == 0x0F)
      cpu->f |= FLAG_H;
    else
      cpu->f &= ~FLAG_H;
    cpu->c++;
    if (cpu->c == 0)
      cpu->f |= FLAG_Z;
    else
      cpu->f &= ~FLAG_Z;
    cpu->f &= ~FLAG_N;
    break;
  }

  case 0x0E: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->c = arg;
    break;
  }

  case 0x16: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->d = arg;
    break;
  }

  case 0x1E: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->e = arg;
    break;
  }

  case 0x26: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->h = arg;
    break;
  }

  case 0x2E: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->l = arg;
    break;
  }

  case 0x3E: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->a = arg;
    break;
  }

  case 0xC3: {
    uint8_t lo = memory_read(cpu->mem, cpu->pc++);
    uint8_t hi = memory_read(cpu->mem, cpu->pc++);
    cpu->pc = (hi << 8) | lo;

    break;
  }

  case 0xC6: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->f &= ~FLAG_N;

    if ((uint16_t)cpu->a + arg > 0xFF)
      cpu->f |= FLAG_C;
    else
      cpu->f &= ~FLAG_C;

    if ((cpu->a & 0x0F) + (arg & 0x0F) > 0x0F)
      cpu->f |= FLAG_H;
    else
      cpu->f &= ~FLAG_H;

    cpu->a += arg;

    if (cpu->a == 0)
      cpu->f |= FLAG_Z;
    else
      cpu->f &= ~FLAG_Z;

    break;
  }

  case 0xD6: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->f |= FLAG_N;

    if (cpu->a < arg)
      cpu->f |= FLAG_C;
    else
      cpu->f &= ~FLAG_C;

    if ((cpu->a & 0x0F) < (arg & 0x0F))
      cpu->f |= FLAG_H;
    else
      cpu->f &= ~FLAG_H;

    cpu->a -= arg;

    if (cpu->a == 0)
      cpu->f |= FLAG_Z;
    else
      cpu->f &= ~FLAG_Z;

    break;
  }

  case 0xE6: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->a &= arg;

    if (cpu->a == 0)
      cpu->f |= FLAG_Z;
    else
      cpu->f &= ~FLAG_Z;

    cpu->f &= ~FLAG_N;
    cpu->f |= FLAG_H;
    cpu->f &= ~FLAG_C;
    break;
  }

  case 0xEE: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->a ^= arg;

    if (cpu->a == 0)
      cpu->f |= FLAG_Z;
    else
      cpu->f &= ~FLAG_Z;

    cpu->f &= ~(FLAG_N | FLAG_H | FLAG_C);
    break;
  }

  case 0xF6: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->a |= arg;

    if (cpu->a == 0)
      cpu->f |= FLAG_Z;
    else
      cpu->f &= ~FLAG_Z;

    cpu->f &= ~(FLAG_N | FLAG_H | FLAG_C);
    break;
  }

  case 0xFE: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
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

  case 0x0D: {
    if ((cpu->c & 0x0F) == 0x0)
      cpu->f |= FLAG_H;
    else
      cpu->f &= ~FLAG_H;

    cpu->c--;

    if (cpu->c == 0)
      cpu->f |= FLAG_Z;
    else
      cpu->f &= ~FLAG_Z;

    cpu->f |= FLAG_N;
    break;
  }

  case 0xDE: {
    uint8_t arg = memory_read(cpu->mem, cpu->pc++);
    uint8_t carry = (cpu->f & FLAG_C) >> 4;

    if ((cpu->a & 0x0F) - (arg & 0x0F) - carry < 0)
      cpu->f |= FLAG_H;
    else
      cpu->f &= ~FLAG_H;

    if ((int)cpu->a - (int)arg - (int)carry < 0)
      cpu->f |= FLAG_C;
    else
      cpu->f &= ~FLAG_C;

    cpu->a -= arg + carry;

    if (cpu->a == 0)
      cpu->f |= FLAG_Z;
    else
      cpu->f &= ~FLAG_Z;

    cpu->f |= FLAG_N;
    break;
  }

  case 0xC5: {
    memory_write(cpu->mem, --cpu->sp, cpu->b);
    memory_write(cpu->mem, --cpu->sp, cpu->c);

    break;
  }

  case 0xC1: {
    cpu->c = memory_read(cpu->mem, cpu->sp++);
    cpu->b = memory_read(cpu->mem, cpu->sp++);

    break;
  }

  case 0xD5: {
    memory_write(cpu->mem, --cpu->sp, cpu->d);
    memory_write(cpu->mem, --cpu->sp, cpu->e);

    break;
  }

  case 0xD1: {
    cpu->e = memory_read(cpu->mem, cpu->sp++);
    cpu->d = memory_read(cpu->mem, cpu->sp++);

    break;
  }

  case 0xE5: {
    memory_write(cpu->mem, --cpu->sp, cpu->h);
    memory_write(cpu->mem, --cpu->sp, cpu->l);

    break;
  }

  case 0xE1: {
    cpu->l = memory_read(cpu->mem, cpu->sp++);
    cpu->h = memory_read(cpu->mem, cpu->sp++);

    break;
  }

  case 0xF5: {
    memory_write(cpu->mem, --cpu->sp, cpu->a);
    memory_write(cpu->mem, --cpu->sp, cpu->f);

    break;
  }

  case 0xF1: {
    cpu->f = memory_read(cpu->mem, cpu->sp++) & 0xF0;
    cpu->a = memory_read(cpu->mem, cpu->sp++);

    break;
  }

  case 0x20: {
    int8_t arg = memory_read(cpu->mem, cpu->pc++);

    if ((cpu->f & FLAG_Z) == 0)
      cpu->pc += arg;

    break;
  }
  case 0x28: {
    int8_t arg = memory_read(cpu->mem, cpu->pc++);

    if ((cpu->f & FLAG_Z) != 0)
      cpu->pc += arg;

    break;
  }

  case 0x30: {
    int8_t arg = memory_read(cpu->mem, cpu->pc++);

    if ((cpu->f & FLAG_C) == 0)
      cpu->pc += arg;

    break;
  }

  case 0x38: {
    int8_t arg = memory_read(cpu->mem, cpu->pc++);

    if ((cpu->f & FLAG_C) != 0)
      cpu->pc += arg;

    break;
  }

  case 0x18: {
    int8_t arg = memory_read(cpu->mem, cpu->pc++);
    cpu->pc += arg;

    break;
  }

  case 0xCD: {
    uint8_t lo = memory_read(cpu->mem, cpu->pc++);
    uint8_t hi = memory_read(cpu->mem, cpu->pc++);
    uint16_t target_addr = (hi << 8) | lo;

    memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
    memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);

    cpu->pc = target_addr;

    break;
  }

  case 0xC9: {
    uint8_t lo = memory_read(cpu->mem, cpu->pc++);
    uint8_t hi = memory_read(cpu->mem, cpu->pc++);
    cpu->pc = (hi << 8) | lo;

    break;
  }

  case 0xC0: {
    if ((cpu->f & FLAG_Z) == 0) {
      uint8_t lo = memory_read(cpu->mem, cpu->sp++);
      uint8_t hi = memory_read(cpu->mem, cpu->sp++);
      cpu->pc = (hi << 8) | lo;
    }

    break;
  }

  case 0xC8: {
    if ((cpu->f & FLAG_Z) != 0) {
      uint8_t lo = memory_read(cpu->mem, cpu->sp++);
      uint8_t hi = memory_read(cpu->mem, cpu->sp++);
      cpu->pc = (hi << 8) | lo;
    }

    break;
  }

  case 0xD0: {
    if ((cpu->f & FLAG_C) == 0) {
      uint8_t lo = memory_read(cpu->mem, cpu->sp++);
      uint8_t hi = memory_read(cpu->mem, cpu->sp++);
      cpu->pc = (hi << 8) | lo;
    }

    break;
  }

  case 0xD8: {
    if ((cpu->f & FLAG_C) != 0) {
      uint8_t lo = memory_read(cpu->mem, cpu->sp++);
      uint8_t hi = memory_read(cpu->mem, cpu->sp++);
      cpu->pc = (hi << 8) | lo;
    }

    break;
  }

  case 0xC4: {
    uint8_t lo = memory_read(cpu->mem, cpu->pc++);
    uint8_t hi = memory_read(cpu->mem, cpu->pc++);
    uint16_t target_addr = (hi << 8) | lo;

    if ((cpu->f & FLAG_Z) == 0) {
      memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
      memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);
      cpu->pc = target_addr;
    }

    break;
  }

  case 0xCC: {
    uint8_t lo = memory_read(cpu->mem, cpu->pc++);
    uint8_t hi = memory_read(cpu->mem, cpu->pc++);
    uint16_t target_addr = (hi << 8) | lo;

    if ((cpu->f & FLAG_Z) != 0) {
      memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
      memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);
      cpu->pc = target_addr;
    }

    break;
  }

  case 0xD4: {
    uint8_t lo = memory_read(cpu->mem, cpu->pc++);
    uint8_t hi = memory_read(cpu->mem, cpu->pc++);
    uint16_t target_addr = (hi << 8) | lo;

    if ((cpu->f & FLAG_C) == 0) {
      memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
      memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);
      cpu->pc = target_addr;
    }

    break;
  }

  case 0xDC: {
    uint8_t lo = memory_read(cpu->mem, cpu->pc++);
    uint8_t hi = memory_read(cpu->mem, cpu->pc++);
    uint16_t target_addr = (hi << 8) | lo;

    if ((cpu->f & FLAG_C) != 0) {
      memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
      memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);
      cpu->pc = target_addr;
    }

    break;
  }

  case 0xE9: {
    cpu->pc = (cpu->h << 8) | cpu->l;

    break;
  }

  case 0xC7: {
    memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
    memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);

    cpu->pc = 0x0000;

    break;
  }

  case 0xCF: {
    memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
    memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);

    cpu->pc = 0x0008;

    break;
  }

  case 0xD7: {
    memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
    memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);

    cpu->pc = 0x0010;

    break;
  }

  case 0xDF: {
    memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
    memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);

    cpu->pc = 0x0018;

    break;
  }

  case 0xE7: {
    memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
    memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);

    cpu->pc = 0x0020;

    break;
  }

  case 0xEF: {
    memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
    memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);

    cpu->pc = 0x0028;

    break;
  }

  case 0xF7: {
    memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
    memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);

    cpu->pc = 0x0030;

    break;
  }

  case 0xFF: {
    memory_write(cpu->mem, --cpu->sp, cpu->pc >> 8);
    memory_write(cpu->mem, --cpu->sp, cpu->pc & 0xFF);

    cpu->pc = 0x0038;

    break;
  }

  default:
    fprintf(stderr, "0x%02X, 0x%04X\n", opcode, cpu->pc - 1);
    break;
  }
}
