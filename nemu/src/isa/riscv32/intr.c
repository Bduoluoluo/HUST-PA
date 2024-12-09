#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  printf("aaaaaaaaaaaaaaaaaaaaa %d\n", stvec);

  rtl_li(&sepc, epc);
  rtl_li(&scause, NO);
  rtl_jr(&stvec);
}

bool isa_query_intr(void) {
  return false;
}
