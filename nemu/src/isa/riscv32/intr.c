#include "rtl/rtl.h"

#define IRQ_TIMER 0x80000005

void raise_intr(uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  rtl_li(&sepc, epc);
  rtl_li(&scause, NO);
  // sstatus = (sstatus & 0xffffffdd) | ((sstatus & 0x2) << 4);
  cpu.sstatus.SPIE = cpu.sstatus.SIE;
  cpu.sstatus.SIE = 0;
  rtl_jr(&stvec);
}

bool isa_query_intr(void) {
  if (cpu.INTR && (cpu.sstatus.SIE)) {
    printf("%d %d a\n", cpu.sstatus.SIE, cpu.sstatus.SPIE);
    cpu.INTR = false;
    raise_intr(IRQ_TIMER, cpu.pc);
    return true;
  }

  return false;
}
