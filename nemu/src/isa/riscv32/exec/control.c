#include "cpu/exec.h"

make_EHelper (jal) {
  rtl_li(&ir, decinfo.seq_pc);
  rtl_sr(id_dest->reg, &ir, 4);
  rtl_addi(&s0, &id_src->val, decinfo.seq_pc);
  rtl_jr(&s0);

  print_asm_template2(jal);
}

make_EHelper (jalr) {
  rtl_li(&ir, decinfo.seq_pc);
  rtl_sr(id_dest->reg, &ir, 4);
  rtl_add(&s0, &id_src->val, &id_src2->val);
  rtl_andi(&s1, &s0, ~1);
  rtl_jr(&s1);

  print_asm_template3(jalr);
}

// opcode 1100011
// fun3 000
make_EHelper (beq) {
  rtl_addi(&s0, &id_dest->val, cpu.pc);
  rtl_jrelop(RELOP_EQ, &id_src->val, &id_src2->val, s0);

  print_asm_template3(beq);
}

// fun3 001
make_EHelper (bne) {
  rtl_addi(&s0, &id_dest->val, cpu.pc);
  rtl_jrelop(RELOP_NE, &id_src->val, &id_src2->val, s0);

  print_asm_template3(bne);
}
// --------------