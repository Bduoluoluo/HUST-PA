#include "cpu/exec.h"

make_EHelper (lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper (auipc) {
  rtl_addi(&s0, &id_src->val, cpu.pc);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template2(auipc);
}

// opcode 0010011
// fun3 000
make_EHelper (addi) {
  rtl_add(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(addi);
}

// fun3 001
// fun7 0000000
make_EHelper (slli) {
  rtl_shl(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(slli);
}
// --------

// fun3 011
make_EHelper (sltiu) {
  rtl_setrelop(RELOP_LTU, &s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);
  
  print_asm_template3(sltiu);
}

// fun3 100
make_EHelper (xori) {
  rtl_xor(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(xori);
}

// fun3 101
// fun7 0000000
make_EHelper (srli) {
  rtl_shr(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(srli);
}

// fun7 0100000
make_EHelper (srai) {
  rtl_sar(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(srai);
}
// --------

// fun3 110
make_EHelper (ori) {
  rtl_or(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(ori);
}

// fun3 111
make_EHelper (andi) {
  rtl_and(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(andi);
}
// --------------

// opcode 0110011
// fun3 000
// fun7 0000000
make_EHelper (add) {
  rtl_add(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(add);
}

// fun7 0100000
make_EHelper (sub) {
  rtl_sub(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(sub);
}
// --------

// fun3 001
// fun7 0000000
make_EHelper (sll) {
  rtl_andi(&s1, &id_src2->val, 0b11111);
  rtl_shl(&s0, &id_src->val, &s1);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(sll);
}
// --------

// fun3 010
// fun7 0000000
make_EHelper (slt) {
  rtl_setrelop(RELOP_LT, &s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);
  
  print_asm_template3(slt);
}
// --------

// fun3 011
// fun7 0000000
make_EHelper (sltu) {
  rtl_setrelop(RELOP_LTU, &s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);
  
  print_asm_template3(sltu);
}
// --------

// fun3 100
// fun7 0000000
make_EHelper (xor) {
  rtl_xor(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(xor);
}
// --------

// fun3 101
// fun7 0000000
make_EHelper (srl) {
  rtl_andi(&s1, &id_src2->val, 0b11111);
  rtl_shr(&s0, &id_src->val, &s1);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(srl);
}

// fun7 0100000
make_EHelper (sra) {
  rtl_andi(&s1, &id_src2->val, 0b11111);
  rtl_sar(&s0, &id_src->val, &s1);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(sra);
}
// --------

// fun3 110
// fun7 0000000
make_EHelper (or) {
  rtl_or(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(or);
}
// --------

// fun3 111
// fun7 0000000
make_EHelper (and) {
  rtl_and(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(and);
}
// --------
// --------------