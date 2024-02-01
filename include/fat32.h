#ifndef __FAT_32_H__
#define __FAT_32_H__

#include <stdint.h>

struct ebp_fat32_t {
    uint32_t BPB_fat_size_32;
    uint16_t BPB_flags;
    uint16_t BPB_fat_version;
    uint32_t BPB_root_cluster_number;
    uint16_t BPB_fsinfo_sector_number;
    uint16_t BPB_backup_sector_bootsector;
    uint8_t BPB_reserved[12];
    uint8_t BS_drive_number;
    uint8_t BS_reserved;
    uint8_t BS_boot_signature;
    uint32_t BS_volume_id;
    uint8_t BS_volume_label[11];
    uint8_t BS_filesystem_type[8];
    uint8_t BS_bootcode32[420];
    uint16_t BS_bootsignature;

}__attribute__((__packed__));

#endif