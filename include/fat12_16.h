#ifndef __FAT_12_16_H__
#define __FAT_12_16_H__

#include <stdint.h>

struct ebp_fat12_16_t {
    uint8_t BS_drive_number;
    uint8_t BS_reserved;
    uint8_t BS_boot_signature;
    uint32_t BS_volume_id;
    uint8_t BS_volume_label[11];
    uint8_t BS_filesystem_type[8];
    uint8_t BS_bootcode[448];
    uint16_t BS_bootsignature;
};

#endif