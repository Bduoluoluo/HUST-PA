#include "monitor/expr.h"

int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  // /* Receive commands from user. */
  // ui_mainloop(is_batch_mode);

  char buf[1024] = "(1)";
  // scanf("%s", buf);
  bool success;
  uint32_t val = expr(buf, &success);
  if (success == false) printf("bad expression!\n");
  else printf("%u\n", val);

  return 0;
}
