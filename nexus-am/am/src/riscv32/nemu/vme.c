#include <am.h>
#include <riscv32.h>
#include <nemu.h>

#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN = {};
static PTE kptabs[(PMEM_SIZE + MMIO_SIZE) / PGSIZE] PG_ALIGN = {};
static void* (*pgalloc_usr)(size_t) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static _Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0x80000000u, .end = (void*)(0x80000000u + PMEM_SIZE)},
  {.start = (void*)MMIO_BASE,   .end = (void*)(MMIO_BASE + MMIO_SIZE)}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

static inline void set_satp(void *pdir) {
  asm volatile("csrw satp, %0" : : "r"(0x80000000 | ((uintptr_t)pdir >> 12)));
}

int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  // make all PDEs invalid
  int i;
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = ((uintptr_t)ptab >> PGSHFT << 10) | PTE_V;

      // fill PTE
      PTE pte = (PGADDR(pdir_idx, 0, 0) >> PGSHFT << 10) | PTE_V | PTE_R | PTE_W | PTE_X;
      PTE pte_end = (PGADDR(pdir_idx + 1, 0, 0) >> PGSHFT << 10) | PTE_V | PTE_R | PTE_W | PTE_X;
      for (; pte < pte_end; pte += (1 << 10)) {
        *ptab = pte;
        ptab ++;
      }
    }
  }

  set_satp(kpdirs);
  vme_enable = 1;

  return 0;
}

int _protect(_AddressSpace *as) {
  PDE *updir = (PDE*)(pgalloc_usr(1));
  as->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  return 0;
}

void _unprotect(_AddressSpace *as) {
}

static _AddressSpace *cur_as = NULL;
void __am_get_cur_as(_Context *c) {
  c->as = cur_as;
}

void __am_switch(_Context *c) {
  if (vme_enable) {
    set_satp(c->as->ptr);
    cur_as = c->as;
  }
}

int _map(_AddressSpace *as, void *va, void *pa, int prot) {
  //printf("_map vaddr:0x%x paddr:0x%x\n",va,pa);
  if(((uint32_t)pa>>12<<12)!=(uint32_t)pa){
    // printf("_map pa error:0x%x\n",pa);
    // assert(0);
  }
  uint32_t* pdir = (uint32_t*)(as->ptr);
  uint32_t pde = pdir[PDX(va)];
  //printf("PDX[0x%x] PTX[0x%x] OFF[0x%x] \n",PDX(va),PTX(va),OFF(va));
  
  if((pde&PTE_V)==0){
    uint32_t *new_pg = pgalloc_usr(1);
    // memset(new_pg,0,PGSIZE);
    pdir[PDX(va)] = ((uint32_t)new_pg>>12<<10) | PTE_V;
    pde=pdir[PDX(va)];
  }
  //printf("pde:%x\n",pde);
  uint32_t *ptab = PTE_ADDR(pde);
  uint32_t pte = ptab[PTX(va)];
  //printf("pte:0x%x\n",pte);
  if((pte&PTE_V)==0){
    ptab[PTX(va)]=((((uint32_t)pa)&(~0xfff))>>2)|PTE_V;
    pte = ptab[PTX(va)];
    // printf("_map vaddr 0x%x =pde:0x%x(0x%x,[%d])=pte:0x%x(0x%x[%d])= paddr:0x%x\n",va,pde,pdir,PDX(va),pte,ptab,PTX(va),pa);
    return 0;
  }else{
    // printf("pte already valid: pde:0x%x pte:0x%x vaddr:0x%x paddr:0x%x\n",pde,pte,va,pa);
    return -1;
  }

  //uint32_t *pte = ((uint32_t*)(as->ptr))[PDX(va)];
  // if(!(((uint32_t)pte)&PTE_V)){
  //   uint32_t *new_pg = pgalloc_usr(1);
  //   memset(new_pg,0,PGSIZE);
  //   ((uint32_t*)(as->ptr))[PDX(va)] = ((uint32_t)new_pg) | PTE_V;
  //   pte = ((uint32_t*)(as->ptr))[PDX(va)];
  // }
  // uint32_t* pgentry = ((uint32_t*)(PTE_ADDR(pte)))[PTX(va)];
  // if((uint32_t)pgentry&PTE_V){
  //   return PTE_ADDR(pgentry);
  // }
  // pa=pgalloc_usr(1);
  // ((uint32_t*)(PTE_ADDR(pte)))[PTX(va)]=((uint32_t)pa)|PTE_V;
  // return pa;
}

_Context *_ucontext(_AddressSpace *as, _Area ustack, _Area kstack, void *entry, void *args) {
  _Context* cp = (_Context *)((uint32_t *)ustack.end - 35);
  cp->epc = (uintptr_t)entry;
  cp->GPR2 = 0;
  cp->GPR3 = 0;
  cp->as = as;

  return cp;
}
