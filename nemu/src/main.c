#include "monitor/expr.h"

int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  // ui_mainloop(is_batch_mode);

  char str[65536];
  bool flag = 0;
  scanf("%s", str);
  uint32_t ans = expr(str, &flag);
  if (flag == false) printf("Failed!");
  else printf("%u\n", ans);

  return 0;
}
