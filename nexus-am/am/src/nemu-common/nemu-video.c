#include <am.h>
#include <amdev.h>
#include <nemu.h>

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      uint32_t screensize = inl(SCREEN_ADDR);
      info->width = screensize >> 16;
      info->height = screensize & 0xffff;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;

      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      } else {
        int p = 0;
        for (int i = 0; i < ctl->w; i ++)
          for (int j = 0; j < ctl->h; j ++) {
            outl(FB_ADDR + (ctl->x + i) * 1600 + (ctl->y + j) * 4, 0x00ff0000);
            // ctl->pixels[p]
            p += 4;
          }
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {

}
