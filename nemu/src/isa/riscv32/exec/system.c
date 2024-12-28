#include "cpu/exec.h"

// fun3 000
make_EHelper (ecall) {
    raise_intr(reg_l(17), cpu.pc);

    print_asm("ecall");
}

make_EHelper (sret) {
    // sstatus = (sstatus & 0xffffffdd) | ((sstatus & 0x20) >> 4) | 0x20;
    cpu.sstatus.SIE = cpu.sstatus.SPIE;
    cpu.sstatus.SPIE = 1;
    // printf("%d %d sret\n", cpu.sstatus.SIE, cpu.sstatus.SPIE);
    rtl_j(sepc + 4);

    print_asm("sret");
}
// --------

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