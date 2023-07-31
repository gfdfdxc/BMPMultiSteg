//
// Created by gfdfdxc.
//

#ifndef FINAL_STEG_H
#define FINAL_STEG_H

#include "common.h"
#include "select.h"

int getNodeLength(FILENAMENODE *head);
STEG_HEADER* create_steg_header(char enc, short all_bmp_number, short this_bmp_number, int all_steg_file_size,
                               int this_steg_file_size,uint32_t all_steg_file_crc32);
int lsb_hide_to_file(FILENAMENODE *filenameList,char* file_to_steg);
int lsb_hide_binary(bmp_img *img,size_t partFileSize,unsigned char *raw_buffer, STEG_HEADER* steg_header);
int mainSteg();
#endif //FINAL_STEG_H
