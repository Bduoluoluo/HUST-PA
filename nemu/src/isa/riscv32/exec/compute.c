#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper (addi) {
  rtl_addi(&s0, &id_src->val, id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);
}