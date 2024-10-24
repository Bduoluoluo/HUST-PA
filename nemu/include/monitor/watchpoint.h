#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

  uint32_t last_val;
  char expr[1024];

} WP;

void new_wp (const char *, uint32_t);
void free_wp (int);
void watchpoint_display ();

#endif
