#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EMPTY, EMPTY, EXW(ld, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EMPTY, EMPTY, EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

// opcode 0010011
static OpcodeEntry iopt_table [8] = {
  EX(addi), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper (iopt) {
  decinfo.width = iopt_table[decinfo.isa.instr.funct3].width;
  idex(pc, &iopt_table[decinfo.isa.instr.funct3]);
}

// opcode 0110011
static OpcodeEntry add_sub_table [2] = {
  EX(add), EMPTY
};

static make_EHelper (add_sub) {
  switch (decinfo.isa.instr.funct7) {
    case 0x0000000: idex(pc, &add_sub_table[0]); break;
    default: assert(0);
  }
}

static OpcodeEntry ropt_table [8] = {
  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper (ropt) {
  printf("aaaaaaaaaaaaaaaa\n");
  decinfo.width = ropt_table[decinfo.isa.instr.funct3].width;
  idex(pc, &ropt_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(I, iopt), IDEX(U, auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, IDEX(R, ropt), IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ EMPTY, IDEX(I, jalr), EX(nemu_trap), IDEX(J, jal), EMPTY, EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
