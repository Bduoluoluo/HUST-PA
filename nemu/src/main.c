#include "monitor/expr.h"
#include <readline/readline.h>
#include <readline/history.h>

int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  // ui_mainloop(is_batch_mode);

  char* str = NULL;
  bool flag = 0;
  str = readline("");
  uint32_t ans = expr(str, &flag);
  if (flag == false) printf("Failed!\n");
  else printf("%u\n", ans);

  return 0;
}
