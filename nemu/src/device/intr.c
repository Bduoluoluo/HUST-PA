#include "nemu.h"

void dev_raise_intr() {
    printf("aaaaaaaaaaa\n");
    cpu.INTR = true;
}
