#ifndef __RISCV32_RTL_H__
#define __RISCV32_RTL_H__

#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t epc);

static inline void rtl_lr(rtlreg_t* dest, int r, int width) {
  if (r != 0) { rtl_mv(dest, &reg_l(r)); }
  else { rtl_li(dest, 0); }
}

static inline void rtl_sr(int r, const rtlreg_t *src1, int width) {
  if (r != 0) { rtl_mv(&reg_l(r), src1); }
}

static inline void rtl_csr_lr (rtlreg_t* dest, int r, int width) {
  rtl_mv(dest, get_csr(r));
}

static inline void rtl_csr_sr(int r, const rtlreg_t *src1, int width) {
  rtl_mv(get_csr(r), src1);
}

#endif
