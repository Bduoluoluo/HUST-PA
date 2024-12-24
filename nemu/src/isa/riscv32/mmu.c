#include "nemu.h"

paddr_t page_translate (vaddr_t addr) {
  if (((satp >> 31) & 1) == 0) return addr;
  uint32_t vpn1 = addr >> 22, vpn2 = (addr >> 12) & 0x3ff, offset = addr & 0xfff;
  uint32_t pte = paddr_read((satp << 12) + vpn1 * 4, 4);
  assert(pte & 1);
  pte = paddr_read(((pte & ~0x3ff) << 2) + vpn2 * 4, 4);
  assert(pte & 1);
  return (((pte & ~0x3ff) << 2) + offset);
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if ((addr >> 12) != ((addr + len - 1) >> 12)) {
    assert(0);
  } else {
    paddr_t paddr = page_translate(addr);
    return paddr_read(paddr, len);
  }
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if ((addr >> 12) != ((addr + len - 1) >> 12)) {
    assert(0);
  } else {
    paddr_t paddr = page_translate(addr);
    paddr_write(paddr, data, len);
  }
}
