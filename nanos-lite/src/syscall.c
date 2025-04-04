#include "common.h"
#include "syscall.h"
#include "fs.h"
#include "proc.h"

extern void naive_uload(PCB *pcb, const char *filename);
extern int mm_brk (uintptr_t brk, intptr_t increment);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_exit:
      // naive_uload(NULL, "/bin/init");
      _halt(0);
      c->GPRx = 0;
      break;
    case SYS_yield:
      _yield();
      c->GPRx = 0;
      break;
    case SYS_open:
      c->GPRx = fs_open((const char *)a[1], a[2], a[3]);
      break;
    case SYS_read:
      c->GPRx = fs_read(a[1], (void *)a[2], a[3]);
      break;
    case SYS_write:
      c->GPRx = fs_write(a[1], (const void *)a[2], a[3]);
      break;
    case SYS_close:
      c->GPRx = fs_close(a[1]);
      break;
    case SYS_lseek:
      c->GPRx = fs_lseek(a[1], a[2], a[3]);
      break;
    case SYS_brk:
      c->GPRx = mm_brk(a[1], a[2]);
      break;
    case SYS_execve:
      naive_uload(NULL, (const char *) a[1]);
      c->GPRx = 0;
      break;
    default:
      panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
