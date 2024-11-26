#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper (auipc) {
  rtl_addi(&s0, &id_src->val, decinfo.seq_pc);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template1(auipc);
}

make_EHelper (addi) {
  rtl_addi(&s0, &id_src->val, id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template2(addi);
}