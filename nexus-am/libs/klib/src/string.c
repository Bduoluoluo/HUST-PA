#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  while (*s != '\0') {
    len ++;
    s ++;
  }
  return len;
}

char* strcpy(char* dst, const char* src) {
  char* ret = dst;
  while (*src != '\0') {
    *dst = *src;
    dst ++;
    src ++;
  }
  *dst = '\0';
  return ret;
}

char* strncpy(char* dst, const char* src, size_t n) {
  char* ret = dst;
  while (n && *src != '\0') {
    *dst = *src;
    dst ++;
    src ++;
    n --;
  }
  while (n) {
    *dst = '\0';
    dst ++;
    n --;
  }
  return ret;
}

char* strcat(char* dst, const char* src) {
  char* ret = dst;
  while (*dst != '\0') dst ++;
  strcpy(dst, src);
  return ret;
}

int strcmp(const char* s1, const char* s2) {
  while (1) {
    if (*s1 < *s2) return -1;
    else if (*s1 > *s2) return 1;
    if (*s1 == '\0') break;
    s1 ++;
    s2 ++;
  }
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  while (n) {
    if (*s1 < *s2) return -1;
    else if (*s1 > *s2) return 1;
    if (*s1 == '\0') break;
    s1 ++;
    s2 ++;
    n --;
  }
  return 0;
}

void* memset(void* v, int c, size_t n) {
  unsigned char* ptr = (unsigned char*) v;
  while (n) {
    *ptr = (unsigned char) c;
    ptr ++;
    n --;
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  printf("aaaaaaa\n");
  char* dest = (char*) out;
  char* src = (char*) in;
  while (n) {
    *dest = *src;
    dest ++;
    src ++;
    n --;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  char* p1 = (char*) s1;
  char* p2 = (char*) s2;
  while (n) {
    if (*p1 < *p2) return -1;
    else if (*p1 > *p2) return 1;
    p1 ++;
    p2 ++;
    n --;
  }
  return 0;
}

#endif
