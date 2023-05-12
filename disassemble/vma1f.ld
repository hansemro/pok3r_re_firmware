
OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY {
  bootloader (rx)  : ORIGIN = 0x1f000000, LENGTH = 8K
}

IMAGE_BASE = 0x1f000000;

SECTIONS {
    .text IMAGE_BASE : {
        *(.text)
    } > bootloader
}
