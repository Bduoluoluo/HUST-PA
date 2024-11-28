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

// fun3 011
make_EHelper (sltiu) {
  rtl_setrelop(RELOP_LTU, &s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);
  
  print_asm_template3(sltiu);
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

// fun3 110
// fun7 0000000
make_EHelper (or) {
  rtl_or(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(or);
}
// --------------