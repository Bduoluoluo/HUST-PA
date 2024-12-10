#include "proc.h"
#include "riscv32.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

uint8_t tmp[32768];

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf_header;
  Elf_Phdr prog_header;

  ramdisk_read(&elf_header, 0, sizeof(Elf_Ehdr));
  Elf32_Off phoff = elf_header.e_phoff;
  for (Elf32_Half i = 0; i < elf_header.e_phnum; i ++) {
    ramdisk_read(&prog_header, phoff, sizeof(Elf_Phdr));
    if (prog_header.p_type != PT_LOAD) continue;

    printf("aaaaaaaaaaaa %x\n", prog_header.p_paddr + prog_header.p_memsz);

    uintptr_t segoff = prog_header.p_offset;
    Elf32_Word j = 0;
    ramdisk_read(tmp, segoff, prog_header.p_filesz);
    for (; j < prog_header.p_filesz; j ++)
      outb(prog_header.p_vaddr + j, tmp[j]);
    for (; j < prog_header.p_memsz; j ++)
      outb(prog_header.p_vaddr + j, 0);

    phoff += sizeof(Elf_Phdr);
  }

  return elf_header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
