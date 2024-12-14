#include "common.h"
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

  int kbd = read_key();
  if (kbd != _KEY_NONE) {
    if (kbd & 0x8000) sprintf(event, "kd %s\n", keyname[kbd ^ 0x8000]);
    else sprintf(event, "ku %s\n", keyname[kbd]);
  } else {
    sprintf(event, "t %d\n", uptime());    
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
  _DEV_VIDEO_INFO_t info;
  _io_read(_DEV_VIDEO, _DEVREG_VIDEO_INFO, &info, sizeof(_DEV_VIDEO_INFO_t));
}
