#include "proc.h"
#include "fs.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

uint8_t tmp[1048576];

extern void* new_page (size_t nr_page);

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);

  Elf_Ehdr elf_header;
  Elf_Phdr prog_header;

  fs_lseek(fd, 0, SEEK_SET);
  fs_read(fd, &elf_header, sizeof(Elf_Ehdr));
  Elf32_Off phoff = elf_header.e_phoff;
  for (Elf32_Half i = 0; i < elf_header.e_phnum; i ++) {
    fs_lseek(fd, phoff, SEEK_SET);
    fs_read(fd, &prog_header, sizeof(Elf_Phdr));
    if (prog_header.p_type != PT_LOAD) continue;

    uintptr_t segoff = prog_header.p_offset;
    fs_lseek(fd, segoff, SEEK_SET);
    fs_read(fd, tmp, prog_header.p_filesz);

    uintptr_t paddr = new_page(1);
    _map(&(pcb->as), prog_header.p_vaddr, paddr, 0x01 | 0x02 | 0x04 | 0x08 | 0x10);
    memcpy((void*)paddr, tmp, prog_header.p_filesz);
    memset((void*)paddr + prog_header.p_filesz, 0, prog_header.p_memsz - prog_header.p_filesz);

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
  _protect(&(pcb->as));
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
