#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,

  /* TODO: Add more token types */
  TK_DEC,
  TK_NEG,
  TK_HEX,
  TK_REG,
  TK_NEQ,
  TK_AND,
  TK_DEREf,
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},                  // spaces
  {"\\+", '+'},                       // plus
  {"-", '-'},                         // minus
  {"\\*", '*'},                       // multiply
  {"/", '/'},                         // divide
  {"\\(", '('},                       // left bracket
  {"\\)", ')'},                       // right bracket
  {"0[xX][0-9A-Fa-f]+", TK_HEX},      // hexadecimal integer
  {"[0-9]+", TK_DEC},                 // decimal integer
  {"\\$[0-9a-z]+", TK_REG},           // register name
  {"==", TK_EQ},                      // equal
  {"!=", TK_NEQ},                     // not equal
  {"&&", TK_AND},                     // and
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
        
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          default:
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            nr_token ++;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses (int p, int q) {
  if (tokens[p].type != '(' || tokens[q].type != ')')
    return false;
  
  int bracket_sta = 0;
  for (int i = p; i <= q; i ++) {
    if (tokens[i].type == '(') bracket_sta ++;
    else if (tokens[i].type == ')') bracket_sta --;
    if (bracket_sta == 0 && i != q) return false;
  }
  return true;
}

uint32_t eval (int p, int q, bool *success) {
  if (p > q) {
    *success = false;
    return 0;
  } else if (p == q) {
    uint32_t val = 0;
    switch (tokens[p].type) {
      case TK_DEC:
        val = strtoul(tokens[p].str, NULL, 10);
        break;
      case TK_HEX:
        val = strtoul(tokens[p].str, NULL, 16);
        break;
      case TK_REG:
        bool is_reg = true;
        val = isa_reg_str2val(tokens[p].str + 1, &is_reg);
        if (is_reg == false)
          *success = false;
        break;
      default:
        *success = false;
        break;
    }
    return val;
  } else if (check_parentheses(p, q) == true) {
    return eval(p + 1, q - 1, success);
  } else {
    int op_0 = -1, op_1 = -1, op_2 = -1, op, bracket_sta = 0;

    for (int i = p; i <= q; i ++) {
      switch (tokens[i].type) {
        case '(':
          bracket_sta ++;
          break;
        case ')':
          bracket_sta --;
          break;
        case TK_EQ:
        case TK_NEQ:
        case TK_AND:
          if (bracket_sta == 0)
            op_0 = i;
          break;
        case '+':
        case '-':
          if (bracket_sta == 0)
            op_1 = i;
          break;
        case '*':
        case '/':
          if (bracket_sta == 0)
            op_2 = i;
          break;
        default:
          break;
      }

      if (bracket_sta < 0) {
        *success = false;
        return 0;
      }
    }

    if (bracket_sta != 0) {
      *success = false;
      return 0;
    }

    if (op_0 == -1 && op_1 == -1 && op_2 == -1) {
      u_int32_t val = eval(p + 1, q, success);
      if (*success == false)
        return 0;

      switch (tokens[p].type) {
        case TK_NEG:
          val = -val;
          break;
        case TK_DEREf:
          val = isa_vaddr_read(val, 4);
          break;
        default:
          *success = false;
          return 0;
      }

      return val;
    }

    if (op_0 != -1) op = op_0;
    else if (op_1 != -1) op = op_1;
    else op = op_2;

    uint32_t val_l = eval(p, op - 1, success), val_r = eval(op + 1, q, success), val = 0;
    switch (tokens[op].type) {
      case '+':
        val = val_l + val_r;
        break;
      case '-':
        val = val_l - val_r;
        break;
      case '*':
        val = val_l * val_r;
        break;
      case '/':
        val = val_l / val_r;
        break;
      case TK_EQ:
        val = val_l == val_r;
        break;
      case TK_NEQ:
        val = val_l != val_r;
        break;
      case TK_AND:
        val = val_l && val_r;
        break;
      default:
        *success = false;
        return 0;
    }

    return val;
  }
}

bool check_unary_opt (int i) {
  if (i == 0) return true;
  switch (tokens[i - 1].type) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case TK_NEG:
    case TK_EQ:
    case TK_NEQ:
    case TK_AND:
    case TK_DEREf:
      return true;
    default:
      return false;
  }
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */

  for (int i = 0; i < nr_token; i ++) {
    if (tokens[i].type == '-' && check_unary_opt(i))
      tokens[i].type = TK_NEG;
    
    if (tokens[i].type == '*' && check_unary_opt(i))
      tokens[i].type = TK_DEREf;
  }
  
  *success = true;
  uint32_t val = eval(0, nr_token - 1, success);
  if (*success == false)
    return 0;

  return val;
}
