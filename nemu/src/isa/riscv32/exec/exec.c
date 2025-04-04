#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry empty_ = EMPTY;

static OpcodeEntry load_table [8] = {
  EX(lb), EX(lh), EXW(ld, 4), EMPTY, EXW(ld, 1), EXW(ld, 2), EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EXW(st, 1), EXW(st, 2), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

// opcode 1110011
static OpcodeEntry irq_table [] = {
  EX(ecall),  // 000000000000
  EX(sret),   // 000100000010
};

static make_EHelper (irq) {
  switch (decinfo.isa.instr.csr) {
    case 0b000000000000: idex(pc, &irq_table[0]); break;
    case 0b000100000010: idex(pc, &irq_table[1]); break;
    default: idex(pc, &empty_);
  }
}

static OpcodeEntry irq_csr_table [8] = {
  EX(irq), IDEX(I_csr, csrrw), IDEX(I_csr, csrrs), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper (irq_csr) {
  decinfo.width = irq_csr_table[decinfo.isa.instr.funct3].width;
  idex(pc, &irq_csr_table[decinfo.isa.instr.funct3]);
}
// --------------

// opcode 1100011
static OpcodeEntry branch_table [8] = {
  EX(beq), EX(bne), EMPTY, EMPTY, EX(blt), EX(bge), EX(bltu), EX(bgeu)
};

static make_EHelper (branch) {
  decinfo.width = branch_table[decinfo.isa.instr.funct3].width;
  idex(pc, &branch_table[decinfo.isa.instr.funct3]);
}
// --------------

// opcode 0010011
static OpcodeEntry iopt_001_table [] = {
  EX(slli),   // 0000000
};
static OpcodeEntry iopt_101_table [] = {
  EX(srli),   // 0100000
  EX(srai),   // 0100000
};

static make_EHelper (iopt_001) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &iopt_001_table[0]); break;
    default: idex(pc, &empty_);
  }
}
static make_EHelper (iopt_101) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &iopt_101_table[0]); break;
    case 0b0100000: idex(pc, &iopt_101_table[1]); break;
    default: idex(pc, &empty_);
  }
}

static OpcodeEntry iopt_table [8] = {
  EX(addi), IDEX(I_shamt, iopt_001), EX(slti), EX(sltiu), EX(xori), IDEX(I_shamt, iopt_101), EX(ori), EX(andi)
};

static make_EHelper (iopt) {
  decinfo.width = iopt_table[decinfo.isa.instr.funct3].width;
  idex(pc, &iopt_table[decinfo.isa.instr.funct3]);
}
// --------------

// opcode 0110011
static OpcodeEntry ropt_000_table [] = {
  EX(add),    // 0000000
  EX(mul),    // 0000001
  EX(sub),    // 0100000
};
static OpcodeEntry ropt_001_table [] = {
  EX(sll),    // 0000000
  EX(mulh),   // 0000000
};
static OpcodeEntry ropt_010_table [] = {
  EX(slt),    // 0000000
};
static OpcodeEntry ropt_011_table [] = {
  EX(sltu),   // 0000000
};
static OpcodeEntry ropt_100_table [] = {
  EX(xor),    // 0000000
  EX(div),    // 0000001
};
static OpcodeEntry ropt_101_table [] = {
  EX(srl),    // 0000000
  EX(divu),   // 0000001
  EX(sra),    // 0100000
};
static OpcodeEntry ropt_110_table [] = {
  EX(or),     // 0000000
  EX(rem),    // 0000001
};
static OpcodeEntry ropt_111_table [] = {
  EX(and),    // 0000000
  EX(remu),   // 0000001
};

static make_EHelper (ropt_000) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &ropt_000_table[0]); break;
    case 0b0000001: idex(pc, &ropt_000_table[1]); break;
    case 0b0100000: idex(pc, &ropt_000_table[2]); break;
    default: idex(pc, &empty_);
  }
}
static make_EHelper (ropt_001) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &ropt_001_table[0]); break;
    case 0b0000001: idex(pc, &ropt_001_table[1]); break;
    default: idex(pc, &empty_);
  }
}
static make_EHelper (ropt_010) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &ropt_010_table[0]); break;
    default: idex(pc, &empty_);
  }
}
static make_EHelper (ropt_011) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &ropt_011_table[0]); break;
    default: idex(pc, &empty_);
  }
}
static make_EHelper (ropt_100) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &ropt_100_table[0]); break;
    case 0b0000001: idex(pc, &ropt_100_table[1]); break;
    default: idex(pc, &empty_);
  }
}
static make_EHelper (ropt_101) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &ropt_101_table[0]); break;
    case 0b0000001: idex(pc, &ropt_101_table[1]); break;
    case 0b0100000: idex(pc, &ropt_101_table[2]); break;
    default: idex(pc, &empty_);
  }
}
static make_EHelper (ropt_110) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &ropt_110_table[0]); break;
    case 0b0000001: idex(pc, &ropt_110_table[1]); break;
    default: idex(pc, &empty_);
  }
}
static make_EHelper (ropt_111) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &ropt_111_table[0]); break;
    case 0b0000001: idex(pc, &ropt_111_table[1]); break;
    default: idex(pc, &empty_);
  }
}

static OpcodeEntry ropt_table [8] = {
  EX(ropt_000), EX(ropt_001), EX(ropt_010), EX(ropt_011), EX(ropt_100), EX(ropt_101), EX(ropt_110), EX(ropt_111)
};

static make_EHelper (ropt) {
  decinfo.width = ropt_table[decinfo.isa.instr.funct3].width;
  idex(pc, &ropt_table[decinfo.isa.instr.funct3]);
}
// --------------

static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(I, iopt), IDEX(U, auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, IDEX(R, ropt), IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ IDEX(B, branch), IDEX(I, jalr), EX(nemu_trap), IDEX(J, jal), EX(irq_csr), EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
