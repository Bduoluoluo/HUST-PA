#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  _yield();
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
  _yield();
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
  strncpy(buf, dispinfo + offset, len);
  if (strlen(dispinfo + offset) < len) len = strlen(dispinfo + offset);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  _yield();
  offset /= 4;
  len /= 4;
  if (!len) return 0;

  size_t width = screen_width();
  int p = 0;
  for (int i = offset / width; i <= (offset + len - 1) / width; i ++) {
    int l = 0, r = width - 1;
    if (i == offset / width) l = offset % width;
    if (i == (offset + len - 1) / width) r = (offset + len - 1) % width;
    draw_rect(((uint32_t*) buf) + p, l, i, r - l + 1, 1);
    p += r - l + 1;
  }

  return len * 4;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  draw_sync();
  return 0;
}

size_t dispinfo_len;

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n\0\0", screen_width(), screen_height());
  dispinfo_len = strlen(dispinfo);
}
