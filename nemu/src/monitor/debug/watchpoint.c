#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp () {
  if (free_ == NULL)
    assert(0);
  
  WP* wp = free_; // erase wp from free_ link
  free_ = free_->next;
  wp->next = head; // insert wp into head link
  head = wp;

  return wp;
}

void free_wp (WP *wp) {
  if (wp == NULL)
    return;

  if (head == wp) { // wp is head
    head = wp->next;
    wp->next = free_;
    free_ = wp;
  } else {
    WP *point = head;
    while (point->next != NULL) { // erase wp from head link
      if (point->next == wp) {
        point->next = wp->next;
        wp->next = free_; // insert wp into free_ link
        free_ = wp;
        break;
      }
      point = point->next;
    }
  }
}
