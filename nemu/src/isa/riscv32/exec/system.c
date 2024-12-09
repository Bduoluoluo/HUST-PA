#include "cpu/exec.h"

// fun3 001
make_EHelper (csrrw) {
    rtl_csr_sr(id_src2->reg, &id_src->val, 4);
    rtl_sr(id_dest->reg, &id_src2->val, 4);

    print_asm_template3(csrrw);
}

// fun3 010
make_EHelper (csrrs) {
    rtl_or(&s0, &id_src2->val, &id_src->val);
    rtl_csr_sr(id_src2->reg, &s0, 4);
    rtl_sr(id_dest->reg, &id_src2->val, 4);

    print_asm_template3(csrrs);
}