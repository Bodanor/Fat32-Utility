#ifndef __FAT32_H__
#define __FAT32_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

typedef struct fat32_t Fat32;

Fat32 *readFat32(char *path);
void destroy_fat32(Fat32 *fat32);
#endif