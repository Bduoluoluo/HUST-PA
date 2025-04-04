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
        for (int i = 0; i < ctl->h; i ++) {
          int st = (ctl->y + i) * 1600 + ctl->x * 4;
          for (int j = 0; j < ctl->w; j ++) {
            outl(FB_ADDR + st, ctl->pixels[p]);
            st += 4;
            p ++;
          }          
        }

      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {

}
