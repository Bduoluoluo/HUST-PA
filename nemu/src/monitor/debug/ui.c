#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "../../isa/riscv32/include/isa/reg.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si (char *args);

static int cmd_info (char *args);

static int cmd_p (char *args);

static int cmd_x (char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */

  {"si", "Execute N(default 1) instructions in a single step", cmd_si},
  {"info", "Print status of registers when argument is 'r', print infomations of watchpoints when argument is 'w'", cmd_info},
  {"p", "Print the value of the expression EXPR", cmd_p},
  {"x", "Output N consecutive 4-bytes in hexadecimal using the expression EXPR as the starting memory address", cmd_x},
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_x (char *args) {
  printf("%s\n", args);

  char *arg = strtok(NULL, " ");

  

  if (arg == NULL) {
    printf("Input the number of 4-bytes in decimal as 'N'\n");
    return 0;
  }
  int num_4bytes = strtol(arg, NULL, 10);

  char *args_end = args + strlen(args);
  args += strlen(arg) + 1;

  printf("%s %u %u\n", args, args_end);

  if (args >= args_end) {
    printf("Input the starting memory address as 'EXPR'\n");
    return 0;
  }

  

  bool success = false;
  vaddr_t addr = expr(arg, &success);
  if (success == false) {
    printf("The input 'EXPR' is wrong\n");
    return 0;
  }

  for (int i = 0; i < num_4bytes; i ++)
    printf("0x%08x  ", isa_vaddr_read(addr + i * 4, 4));
  printf("\n");

  return 0;
}

static int cmd_p (char *args) {
  if (args == NULL) {
    printf("Input the 'EXPR'\n");
    return 0;
  }

  bool success = false;
  uint32_t val = expr(args, &success);
  if (success == false) {
    printf("The input 'EXPR' is wrong\n");
    return 0;
  }
  printf("0x%08x\n", val);

  return 0;
}

static int cmd_info (char *args) {
  char *arg = strtok(NULL, " ");

  if (strcmp(arg, "r") == 0) {
    isa_reg_display();
  } else
    printf("Unknown command '%s'\n", arg);
  
  return 0;
}

static int cmd_si (char *args) {
  char *arg = strtok(NULL, " ");

  if (arg == NULL) cpu_exec(1);
  else {
    uint64_t num = strtoull(arg, NULL, 10);
    cpu_exec(num);
  }

  return 0;
}

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
