#ifndef __FAT_H__
#define __FAT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "fat32.h"
#include "fat12_16.h"

typedef struct fat_t Fat;

Fat *readFat(char *path);
void destroy_fat(Fat *fat);
#endif