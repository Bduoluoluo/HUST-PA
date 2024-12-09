#ifndef __RISCV32_REG_H__
#define __RISCV32_REG_H__

#include "common.h"

#define PC_START (0x80000000u + IMAGE_START)

typedef struct {
  struct {
    rtlreg_t _32;
  } gpr[32];

  vaddr_t pc;

} CPU_state;

const char *csr_name[] = {
  "stvec", "sepc", "scause", "sstatus"
};

static rtlreg_t stvec;
static rtlreg_t sepc;
static rtlreg_t scause;
static rtlreg_t sstatus;

static inline rtlreg_t* get_csr (uint32_t csr) {
  switch (csr) {
    case 0b000100000101: return &stvec;
    case 0b000101000001: return &sepc;
    case 0b000101000010: return &scause;
    case 0b000100000000: return &sstatus;
    default: panic("csr register undefined!"); return NULL;
  }
}

static inline const char* get_csr_name (uint32_t csr) {
  switch (csr) {
    case 0b000100000101: return csr_name[0];
    case 0b000101000001: return csr_name[1];
    case 0b000101000010: return csr_name[2];
    case 0b000100000000: return csr_name[3];
    default: panic("csr register undefined!"); return NULL;
  }
}

void isa_reg_display ();

static inline int check_reg_index(int index) {
  assert(index >= 0 && index < 32);
  return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)

static inline const char* reg_name(int index, int width) {
  extern const char* regsl[];
  assert(index >= 0 && index < 32);
  return regsl[index];
}

uint32_t isa_reg_str2val (const char *s, bool *success);

#endif
