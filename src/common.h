//
// Created by gfdfdxc.
//

#ifndef FINAL_COMMON_H
#define FINAL_COMMON_H

#include <stdint.h>

extern char dirPath[240];
typedef struct {
    char SOFTWARE_NAME [8];
    char SOFTWARE_VER [3];
    char ENC_FLAG;
    short ALL_BMP_NUMBER;
    short THIS_BMP_NUMBER;
    int ALL_STEG_FILE_SIZE;
    int THIS_STEG_FILE_SIZE;
    uint32_t ALL_STEG_FILE_CRC32;
    uint32_t THIS_STEG_FILE_CRC32;
} STEG_HEADER;
#endif //FINAL_COMMON_H
