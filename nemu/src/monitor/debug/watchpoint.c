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

void new_wp (const char *args, uint32_t val) {
  if (free_ == NULL)
    assert(0);
  
  WP* wp = free_; // erase wp from free_ link
  free_ = free_->next;
  wp->next = head; // insert wp into head link
  head = wp;

  strcpy(wp->expr, args);
  wp->last_val = val;
}

void free_wp (int NO) {
  if (head == NULL) return; // no watchpoint

  WP *wp = NULL;
  if (head->NO == NO) { // NO is head
    wp = head;
    head = wp->next;
    wp->next = free_;
    free_ = wp;
  } else {
    WP *point = head;
    while (point->next != NULL) {
      if (point->next->NO == NO) {
        wp = point->next; // erase wp from head link
        point->next = wp->next;
        wp->next = free_; // insert wp into free_ link
        free_ = wp;
        break;
      }
      point = point->next;
    }
  }
}

void watchpoint_display () {
  WP *point = head;
  while (point != NULL) {
    printf("\nWatchpoint %d:\n", point->NO);
    printf("EXPR: %s\n", point->expr);
    printf("Value: %u\n\n", point->last_val);
    point = point->next;
  }
}

bool check_watchpoint () {
  bool is_changed = false;

  WP *point = head;
  while (point != NULL) {
    bool success = true;
    uint32_t new_val = expr(point->expr, &success);
    if (new_val != point->last_val) {
      printf("\nWatchpoint %d:\n", point->NO);
      printf("EXPR: %s\n", point->expr);
      printf("Old value: %u\n", point->last_val);
      printf("New value: %u\n\n", new_val);

      point->last_val = new_val;
      is_changed = true;
    }

    point = point->next;
  }

  return is_changed;
}