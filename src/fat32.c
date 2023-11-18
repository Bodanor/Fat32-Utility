#include "fat32.h"

static Fat32 *create_fat32(void);

typedef struct bpb_t Bpb;
typedef struct ebp_t Ebp;

struct bpb_t {
    uint8_t BS_JmpBoot[3];
    uint8_t BS_OEMName[8];
    uint16_t BPB_bytes_per_sector;
    uint8_t BPB_sectors_per_cluster;
    uint16_t BPB_reserver_sectors_count;
    uint8_t BPB_fats_numbers;
    uint16_t BPB_root_entries_count;
    uint16_t BPB_total_sectors_16;
    uint8_t BPB_media;
    uint16_t BPB_fat_size_16;
    uint16_t BPB_sectors_per_track;
    uint16_t BPB_heads_number;
    uint32_t BPB_hidden_sectors;
    uint32_t BPB_total_sectors_32;
}__attribute__((__packed__));

struct ebp_t {
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

struct fat32_t {
    Bpb * bpb;
    Ebp * ebp;
};

Fat32 *create_fat32(void)
{
    Fat32 *fat32;

    fat32 = (Fat32*)malloc(sizeof(Fat32));
    if (fat32 == NULL)
        return NULL;
    
    fat32->bpb = (Bpb*)malloc(sizeof(Bpb));
    if (fat32->bpb == NULL)
        return NULL;
    
    fat32->ebp = (Ebp*)malloc(sizeof(Ebp));
    if (fat32->ebp == NULL)
        return NULL;
    
    return fat32;
}

Fat32 *readFat32(char *path)
{
    FILE *fat32_descriptor;
    Fat32 *fat32;

    if (path == NULL)
        return NULL;
    
    fat32_descriptor = fopen(path, "rb");
    if (fat32_descriptor == NULL) {
        perror(strerror(errno));
        return NULL;
    }
    
    fat32 = create_fat32();
    if (fat32 == NULL) {
        perror(strerror(errno));
        return NULL;
    }
    
    if (fread(fat32->bpb, 36, 1, fat32_descriptor) != 1) {
        fprintf(stderr, "Could not retrieve the BPB from the fat32 disk !");
        return NULL;
    }
    if (fread(fat32->ebp, 476, 1, fat32_descriptor) != 1) {
        fprintf(stderr, "Could not retrieve the EBP from the fat32 disk !");
        return NULL;
    }
    
    return fat32;
}

void destroy_fat32(Fat32 *fat32)
{
    free(fat32->bpb);
    free(fat32->ebp);
    free(fat32);
}
