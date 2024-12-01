#include "cpu/exec.h"

make_EHelper (jal) {
  rtl_li(&ir, decinfo.seq_pc);
  rtl_sr(id_dest->reg, &ir, 4);
  rtl_addi(&s0, &id_src->val, cpu.pc);
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

// fun3 100
make_EHelper (blt) {
  rtl_addi(&s0, &id_dest->val, cpu.pc);
  rtl_jrelop(RELOP_LT, &id_src->val, &id_src2->val, s0);

  print_asm_template3(blt);
}

// fun3 101
make_EHelper (bge) {
  rtl_addi(&s0, &id_dest->val, cpu.pc);
  rtl_jrelop(RELOP_GE, &id_src->val, &id_src2->val, s0);

  print_asm_template3(bge);
}

// fun3 110
make_EHelper (bltu) {
  rtl_addi(&s0, &id_dest->val, cpu.pc);
  rtl_jrelop(RELOP_LTU, &id_src->val, &id_src2->val, s0);

  print_asm_template3(bltu);
}

// fun3 111
make_EHelper (bgeu) {
  rtl_addi(&s0, &id_dest->val, cpu.pc);
  rtl_jrelop(RELOP_GEU, &id_src->val, &id_src2->val, s0);

  print_asm_template3(bgeu);
}
// --------------