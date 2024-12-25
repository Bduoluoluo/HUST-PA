#include "memory.h"
#include "proc.h"

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

extern PCB *current;

/* The brk() system call handler. */
int mm_brk(uintptr_t brk, intptr_t increment) {
  // if (current->max_brk < brk) {
  //   for (uintptr_t i = PGROUNDUP(current->max_brk); i <= PGROUNDDOWN(brk); i += PGSIZE) {
  //     uintptr_t pa = (uintptr_t)new_page(1);
  //     _map(&(current->as), (void *)i, (void *)pa, 0);
  //   }

  //   current->max_brk = brk;
  // }

  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
