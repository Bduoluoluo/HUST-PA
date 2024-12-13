#include "common.h"
#include <am.h>
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for (int i = 0; i < len; i ++)
    _putc(((uint8_t *) buf)[i]);
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  char event[32];
  _DEV_INPUT_KBD_t kbd;
  _DEV_TIMER_UPTIME_t uptime;

  _io_read(_DEV_INPUT, _DEVREG_INPUT_KBD, &kbd, sizeof(_DEV_INPUT_KBD_t));
  if (kbd.keycode != _KEY_NONE) {
    if (kbd.keydown) sprintf(event, "kd %s\n", keyname[kbd.keycode]);
    else sprintf(event, "ku %s\n", keyname[kbd.keycode]);
  } else {
    _io_read(_DEV_TIMER, _DEVREG_TIMER_UPTIME, &uptime, sizeof(_DEV_TIMER_UPTIME_t));
    sprintf(event, "t %d\n", uptime.lo);    
  }

  strncpy(buf, event, len);
  if (strlen(event) < len) len = strlen(event);
  return len;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
