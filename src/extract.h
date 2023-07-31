//
// Created by gfdfdxc.
//

#ifndef FINAL_EXTRACT_H
#define FINAL_EXTRACT_H
int extractBMP();
int mainExtract();
#include "common.h"
#include "crc32.h"
STEG_HEADER get_steg_header(char* filename);
CRC32NODE* getCRC32List(STEG_HEADER all_header[], int allFilesCount);
uint32_t selectCRC32(const uint32_t CRC32List[],int allFilesCount);
unsigned char* lsb_get_binary(char* filename,size_t partFileSize);
#endif //FINAL_EXTRACT_H
