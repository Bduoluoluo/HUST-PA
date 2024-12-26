#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t i2s (char *dst, int d, size_t n, size_t width) {
  char str[65];
  size_t ret = 0, len = 0;
  int neg = 0;
  if (d < 0) neg = 1, d = -d;
  else if (d == 0) {
    str[len ++] = '0';
    if (width) width --;
  }
  while (d) {
    str[len ++] = d % 10 + '0';
    d /= 10;
    if (width) width --;
  }
  if (neg) {
    str[len ++] = '-';
    if (width) width --;
  }
  while (width) {
    str[len ++] = '0';
    width --;
  }
  while (n && len) {
    *dst = str[-- len];
    dst ++;
    n --;
    ret ++;
  }
  return ret;
}

size_t hi2s (char *dst, unsigned d, size_t n, size_t width) {
  char str[25];
  size_t ret = 0, len = 0;
  if (d == 0) {
    str[len ++] = '0';
    if (width) width --;
  }
  while (d) {
    if (d % 16 < 10) str[len ++] = d % 16 + '0';
    else str[len ++] = d % 16 - 10 + 'a';
    d /= 16;
    if (width) width --;
  }
  while (width) {
    str[len ++] = '0';
    width --;
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
          char s[65];
          size_t len = i2s(s, num, -1, 0);
          m += len;
          for (size_t i = 0; i < len; i ++) _putc(s[i]);
          break;
        }
        case 'x': {
          unsigned num = va_arg(args, unsigned);
          char s[25];
          size_t len = hi2s(s, num, -1, 0);
          m += len;
          for (size_t i = 0; i < len; i ++) _putc(s[i]);
          break;
        }
        case 'p': {
          unsigned num = va_arg(args, unsigned);
          char s[25];
          size_t len = hi2s(s, num, -1, 8);
          m += len;
          for (size_t i = 0; i < len; i ++) _putc(s[i]);
          break;
        }
        case 's': {
          char *s = va_arg(args, char*);
          size_t len = strlen(s);
          m += len;
          for (size_t i = 0; i < len; i ++) _putc(s[i]);
          break;
        }
        case 'c': {
          char c = va_arg(args, char);
          _putc(c);
          m ++;
          break;
        }
        case '%': {
          _putc('%');
          m ++;
          break;
        }
        case '0': {
          p ++;
          int width = *p - '0';
          p ++;
          if (*p != 'd') return -1;
          if (width <= 0 || width > 9) return -1;
          int num = va_arg(args, int);
          char s[65];
          size_t len = i2s(s, num, -1, width);
          m += len;
          for (size_t i = 0; i < len; i ++) _putc(s[i]);
          break;
        }
        default: return -1;
      }
    } else {
      _putc(*p);
      m ++;
    }
    p ++;
  }

  va_end(args);

  return m;
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
          m += i2s(out + m, num, -1, 0);
          break;
        }
        case 'x': {
          unsigned num = va_arg(args, unsigned);
          m += hi2s(out + m, num, -1, 0);
          break;
        }
        case 'p': {
          unsigned num = va_arg(args, unsigned);
          m += hi2s(out + m, num, -1, 8);
          break;
        }
        case 's': {
          char *s = va_arg(args, char*);
          size_t len = strlen(s);
          strcpy(out + m, s);
          m += len;
          break;
        }
        case 'c': {
          char c = va_arg(args, char);
          out[m ++] = c;
          break;
        }
        case '%': {
          out[m ++] = '%';
          break;
        }
        case '0': {
          p ++;
          int width = *p - '0';
          p ++;
          if (*p != 'd') return -1;
          if (width <= 0 || width > 9) return -1;
          int num = va_arg(args, int);
          m += i2s(out + m, num, -1, width);
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
