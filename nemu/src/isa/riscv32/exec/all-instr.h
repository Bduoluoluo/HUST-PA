#include "cpu/exec.h"

// compute
make_EHelper(lui);
make_EHelper(auipc);
make_EHelper(addi);
make_EHelper(sltiu);
make_EHelper(xori);
make_EHelper(srai);
make_EHelper(andi);
make_EHelper(add);
make_EHelper(sub);
make_EHelper(sll);
make_EHelper(slt);
make_EHelper(sltu);
make_EHelper(xor);
make_EHelper(srl);
make_EHelper(or);
make_EHelper(and);

// control
make_EHelper(jal);
make_EHelper(jalr);
make_EHelper(beq);
make_EHelper(bne);
make_EHelper(blt);
make_EHelper(bge);

// ldst
make_EHelper(ld);
make_EHelper(st);

// muldiv
make_EHelper(mul);
make_EHelper(div);
make_EHelper(rem);

// special
make_EHelper(inv);
make_EHelper(nemu_trap);
