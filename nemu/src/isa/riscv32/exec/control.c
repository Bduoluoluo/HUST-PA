#include "cpu/exec.h"

make_EHelper (jal) {
    rtl_li(&ir, decinfo.seq_pc);
    rtl_sr(id_dest->reg, &ir, 4);
    rtl_add(&s0, &id_src->val, &ir);
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