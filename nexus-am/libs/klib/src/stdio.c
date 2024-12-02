#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t i2s (char *dst, int d, size_t n) {
  char str[65];
  size_t ret = 0, len = 0;
  while (d) {
    str[len ++] = d % 10 + '0';
    d /= 10;
  }
  while (n && len) {
    *dst = str[-- len];
    dst ++;
    n --;
    ret ++;
  }
  return ret;
}

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  size_t m = 0;
  const char *p = fmt;

  va_start(args, fmt);

  while (*p != '\0') {
    if (*p == '%') {
      p ++;
      switch (*p) {
        case 'd': {
          int num = va_arg(args, int);
          m += i2s(out + m, num, -1);
          break;          
        }
        case 's': {
          char *s = va_arg(args, char*);
          size_t len = strlen(s);
          strcpy(out + m, s);
          m += len;
          break;          
        }
        case '%': {
          out[m ++] = '%';
          break;          
        }
        default: return -1;
      }
    } else out[m ++] = *p;
    p ++;
  }
  out[m] = '\0';

  va_end(args);

  return m;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
