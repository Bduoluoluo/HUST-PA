#include "cpu/exec.h"

// opcode 0110011
// fun3 000
// fun7 0000001
make_EHelper (mul) {
  rtl_mul_lo(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(mul);
}
// --------

// fun3 100
// fun7 0000001
make_EHelper (div) {
  rtl_idiv_q(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(div);
}
// --------

// fun3 110
// fun7 0000001
make_EHelper (rem) {
  rtl_idiv_r(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(rem);
}
// --------
// --------------