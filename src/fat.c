#include "fat.h"

static Fat *alloc_fat(void);
static Fat *readVBR(char *path);
static uint8_t check_fat_type(Fat *fat);
static uint8_t validate_bpb(Fat *fat);

typedef struct bpb_t Bpb;

typedef enum {
    FAT16,
    FAT32
} FatType;


struct bpb_t {
    uint8_t BS_JmpBoot[3];
    uint8_t BS_OEMName[8];
    uint16_t BPB_bytes_per_sector;
    uint8_t BPB_sectors_per_cluster;
    uint16_t BPB_reserved_sectors_count;
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


struct fat_t {
    uint32_t root_directory_start_sector;
    uint32_t root_directory_sectors;
    uint32_t data_directory_start_sector;
    uint32_t data_directory_sectors;
    uint32_t fat_sectors;
    FatType type;
    Bpb * bpb;
    void * ebp;
};

Fat *alloc_fat(void)
{
    Fat *fat;

    fat = (Fat*)malloc(sizeof(Fat));
    if (fat == NULL)
        return NULL;
    
    fat->bpb = (Bpb*)malloc(sizeof(Bpb));
    if (fat->bpb == NULL)
        return NULL;
    
    /*
     * We don't know the type of fat we are working with in advance. But thankfully, both fat12/16 and fat32
     * Share the same struct sizes. We therefore have the choice to allocate for either of those struct since they
     * Have the same size. Beneath, I choose fat32
     */

    fat->ebp = (void*)malloc(sizeof(struct ebp_fat32_t));
    if (fat->ebp == NULL)
        return NULL;
    
    return fat;
}

Fat *readVBR(char *path)
{
    FILE *fat_descriptor;
    Fat *fat;

    if (path == NULL)
        return NULL;
    
    fat_descriptor = fopen(path, "rb");
    if (fat_descriptor == NULL) {
        perror(strerror(errno));
        return NULL;
    }
    
    fat = alloc_fat();
    if (fat == NULL) {
        perror(strerror(errno));
        return NULL;
    }
    
    /* Both fat12/16 and fat32 have the same BPB size of 36 bytes */
    if (fread(fat->bpb, 36, 1, fat_descriptor) != 1) {
        fprintf(stderr, "Could not retrieve the BPB from the fat32 disk !");
        return NULL;
    }

    /* The total Boot Sector or Volume Boot record is 512 bytes in size. We have already read 36 from here so 512 - 36 = 476 bytes remained to be read (Which is the EBP)*/
    if (fread(fat->ebp, 476, 1, fat_descriptor) != 1) {
        fprintf(stderr, "Could not retrieve the EBP from the fat32 disk !");
        return NULL;
    }
    
    /* If everything went already, from this point we have read the whole Boot Sector or Volume Boot Record. */
    return fat;
}

static uint8_t check_fat_type(Fat *fat)
{
    if (fat == NULL || fat->bpb == NULL || fat->ebp == NULL)
        return 1;
    
    

}

static uint8_t validate_bpb(Fat *fat)
{
    if (fat == NULL || fat->bpb == NULL || fat->ebp == NULL)
        return 1;
    
    /* Check if the Jump boot code is valid for hte FAT specs */
    if (fat->bpb->BS_JmpBoot[0] == 0xEB && fat->bpb->BS_JmpBoot[2] != 0x90)
    {
       fprintf(stderr, "Invalid Short Jump inside the BPB !");
       return 2;
    }
    else if (fat->bpb->BS_JmpBoot[0] != 0xE9)
    {
        fprintf(stderr, "Invalid Near Jump inside the BPB !");
        return 2;
    }

    /* Check the boot signature. Must be equal to 0xAA55 to be a valid FAT disk */
    if (((int*)fat->ebp)[510] != 0xAA55) {
        fprintf(stderr, "Invalid Boot Signature. This is not a FAT fornated disk !");
        return 3;
    }

    return 0;

}
Fat *readFat(char *path)
{
    Fat *fat;

    fat = readVBR(path);
    if (fat == NULL)
        return NULL;
    
    if (validate_bpb(fat) != 0)
        return NULL;
    
    /* Compute the number of sectors that the fat contains. If fat->bpb->BPB_fat_size_16 is equal to 0, we have to check the fat32 value to cmompute the fat sectors */
    if (fat->bpb->BPB_fat_size_16 != 0)
        fat->fat_sectors = fat->bpb->BPB_fat_size_16 * fat->bpb->BPB_fats_numbers;
    else
        fat->fat_sectors = ((struct ebp_fat32_t*)fat->ebp)->BPB_fat_size_32 * fat->bpb->BPB_fats_numbers;

    fat->root_directory_start_sector = fat->fat_sectors + fat->bpb->BPB_reserved_sectors_count;
    fat->root_directory_sectors = (32 * fat->bpb->BPB_root_entries_count + fat->bpb->BPB_bytes_per_sector - 1) / fat->bpb->BPB_bytes_per_sector;

    fat->data_directory_start_sector = fat->root_directory_start_sector + fat->root_directory_sectors;
    if (fat->bpb->BPB_total_sectors_16 < 0x10000)
        fat->data_directory_sectors = fat->bpb->BPB_total_sectors_16 + fat->root_directory_sectors;
    else
        fat->data_directory_sectors = fat->bpb->BPB_total_sectors_32 + fat->root_directory_sectors;
    
}


void destroy_fat(Fat *fat)
{
    free(fat->bpb);
    free(fat->ebp);
    free(fat);
}
