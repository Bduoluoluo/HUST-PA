#include <am.h>
#include <amdev.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

size_t __am_input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _DEV_INPUT_KBD_t *kbd = (_DEV_INPUT_KBD_t *)buf;
      uint32_t k = inl(KBD_ADDR);
      if (k == _KEY_NONE) {
        kbd->keydown = 0;
        kbd->keycode = _KEY_NONE;
      } else {
        kbd->keydown = k & KEYDOWN_MASK;
        if (kbd->keydown) kbd->keycode = k ^ KEYDOWN_MASK;
        else kbd->keycode = k;        
      }
      return sizeof(_DEV_INPUT_KBD_t);
    }
  }
  return 0;
}
