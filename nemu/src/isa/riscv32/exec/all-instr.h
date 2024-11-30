#include "cpu/exec.h"

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
make_EHelper(sltu);
make_EHelper(xor);
make_EHelper(srl);
make_EHelper(or);
make_EHelper(and);

make_EHelper(jal);
make_EHelper(jalr);
make_EHelper(beq);
make_EHelper(bne);
make_EHelper(bge);

make_EHelper(ld);
make_EHelper(st);

make_EHelper(inv);
make_EHelper(nemu_trap);
