#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

extern size_t serial_write (const void *buf, size_t offset, size_t len);
extern size_t events_read (void *buf, size_t offset, size_t len);
extern size_t fb_write (const void *buf, size_t offset, size_t len);
extern size_t fbsync_write (const void *buf, size_t offset, size_t len);
extern size_t dispinfo_read (void *buf, size_t offset, size_t len);

extern size_t ramdisk_read (void *buf, size_t offset, size_t len);
extern size_t ramdisk_write (const void *buf, size_t offset, size_t len);

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/fb", 0, 0, invalid_read, fb_write},
  {"/dev/fbsync", 0, 0, invalid_read, fbsync_write},
  {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
  {"/dev/events", 0, 0, events_read, invalid_write},
  {"/dev/tty", 0, 0, invalid_read, serial_write},
  #include "files.h"
};

size_t min (size_t x, size_t y) {
  return (x < y) ? x : y;
}

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_open (const char *pathname, int flags, int mode) {
  int fd = -1;
  for (int i = 0; i < NR_FILES; i ++)
    if (strcmp(file_table[i].name, pathname) == 0) {
      fd = i;
      break;
    }
  assert(fd != -1);
  file_table[fd].open_offset = 0;

  return fd;
}

size_t fs_read (int fd, void *buf, size_t len) {
  if (file_table[fd].read != NULL) {
    if (file_table[fd].size) len = min(len, file_table[fd].size - file_table[fd].open_offset);
    len = file_table[fd].read(buf, file_table[fd].open_offset, len);
    if (file_table[fd].size) file_table[fd].open_offset += len;
  } else {
    len = min(len, file_table[fd].size - file_table[fd].open_offset);
    ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    file_table[fd].open_offset += len;
  }
  
  return len;
}

size_t fs_write (int fd, const void *buf, size_t len) {
  if (file_table[fd].write != NULL) {
    if (file_table[fd].size) len = min(len, file_table[fd].size - file_table[fd].open_offset);
    len = file_table[fd].write(buf, file_table[fd].open_offset, len);
    if (file_table[fd].size) file_table[fd].open_offset += len;
  } else {
    len = min(len, file_table[fd].size - file_table[fd].open_offset);
    ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    file_table[fd].open_offset += len;    
  }

  return len;
}

size_t fs_lseek (int fd, size_t offset, int whence) {
  size_t real_offset;
  switch (whence) {
    case SEEK_SET: real_offset = offset; break;
    case SEEK_CUR: real_offset = min(file_table[fd].size, file_table[fd].open_offset + offset); break;
    case SEEK_END: real_offset = min(file_table[fd].size, file_table[fd].size + offset); break;
    default: return -1;
  }
  file_table[fd].open_offset = real_offset;
  return real_offset;
}

int fs_close (int fd) {
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
  file_table[FD_FB].size = screen_height() * screen_width();
  extern size_t dispinfo_len;
  file_table[fs_open("/proc/dispinfo", 0, 0)].size = dispinfo_len;
}
