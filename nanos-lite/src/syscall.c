#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_exit:
      _halt(0);
      break;
    case SYS_yield:
      _yield();
      c->GPRx = 0;
      break;
    case SYS_write:
      Log("aaaaaaaaaaaaaaaaa\n");
      if (a[1] == 1 || a[1] == 2) {
        for (int i = 0; i < a[3]; i ++)
          _putc(((uint8_t *)a[2])[i]);
        c->GPRx = a[3];
      } else
        c->GPRx = -1;
      break;
    case SYS_brk:
      c->GPRx = 0;
      break;
    default:
      panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
