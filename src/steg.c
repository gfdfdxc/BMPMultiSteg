//
// Created by gfdfdxc.
//

#include "steg.h"
#include "common.h"
#include <stdio.h>
#include "select.h"
#include <stdlib.h>
#include <string.h>
#include "libbmp.h"
#include "info.h"
#include "crc32.h"

int mainSteg() {
    puts("+================+");
    puts("|Steg Folder/File|");
    puts("+================+");
    FILENAMENODE *selectedList = selectBMP();
    if (selectedList == NULL) return 0x7;


    printf("Which file do you want to steg?\n");
    char file_to_steg[FILENAME_MAX];
    scanf("%s", file_to_steg);

//    char file_to_steg[FILENAME_MAX]="";
    lsb_hide_to_file(selectedList, file_to_steg);
//    lsb_hide_to_file(image_filename, binary_filename, output_filename);
    return 0;
}


int lsb_hide_binary(bmp_img *img,size_t partFileSize,unsigned char *raw_buffer, STEG_HEADER* steg_header) {//用来隐写一部分二进制，自动会添加文件头
    unsigned long long fullPartFileSize=partFileSize+sizeof(STEG_HEADER);
    printf("part file size is %zu\n", partFileSize);
    printf("fullPartFileSize is %zu\n", fullPartFileSize);
    unsigned char *buffer = (unsigned char *) malloc(partFileSize + sizeof(STEG_HEADER));
    if (buffer == NULL) {
        printf("Error allocating memory for binary file.\n");

        return 0;
    }
    memcpy(buffer, steg_header, sizeof(STEG_HEADER));
    memcpy(buffer+sizeof(STEG_HEADER), raw_buffer, partFileSize);
    size_t bit_index = 0;
    for (size_t y = 0; y < img->img_header.biHeight; y++) {
        for (size_t x = 0; x < img->img_header.biWidth; x++) {
            bmp_pixel *pixel = &img->img_pixels[y][x];

            // 隐藏红色通道
            if (bit_index < fullPartFileSize * 8) {
                pixel->red = (pixel->red & 0xFE) | ((buffer[bit_index >> 3] >> (7 - (bit_index & 7))) & 0x01);
                bit_index++;
            }
            // 隐藏绿色通道
            if (bit_index < fullPartFileSize * 8) {
                pixel->green = (pixel->green & 0xFE) | ((buffer[bit_index >> 3] >> (7 - (bit_index & 7))) & 0x01);
                bit_index++;
            }
            // 隐藏蓝色通道
            if (bit_index < fullPartFileSize * 8) {
                pixel->blue = (pixel->blue & 0xFE) | ((buffer[bit_index >> 3] >> (7 - (bit_index & 7))) & 0x01);
                bit_index++;
            }
            if (bit_index >= fullPartFileSize * 8) {
                break;
            }
        }
    }

    free(buffer);
}

int lsb_hide_to_file(FILENAMENODE *filenameList,char* file_to_steg) {
    int bmp_number = getNodeLength(filenameList);//有几个文件就是几
    printf("You have selected %d files.\n", bmp_number);
    //read file_to_steg to buffer
    FILE *file = fopen(file_to_steg, "rb");
    if (file == NULL) {
        printf("Error opening binary file.\n");
        return 1;
    }
    //get binary size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    printf("steg file size is %ld\n", file_size);
    int each_file_size[bmp_number];//创建一个存放之后每个隐写文件大小的数组
    if (file_size %bmp_number==0) {
    for(int i=0;i<bmp_number;i++){
        each_file_size[i]=file_size / bmp_number;
    }}
    else{
            for(int i=0;i<bmp_number-1;i++){
        each_file_size[i]=file_size / bmp_number;
        each_file_size[bmp_number-1]=file_size - (file_size / bmp_number)*(bmp_number-1);//如果除不尽，最后一个文件要大一点
    }
    }//除了最后一个文件外，每个文件的大小都是一样的

    //get filename in filenameList
    char *filename[bmp_number];
    int i=0;
    FILENAMENODE *tempfilenameList=filenameList;
    while(tempfilenameList!=NULL){
        filename[i]=tempfilenameList->fileName;
        tempfilenameList=tempfilenameList->next;
        i++;
    }//把所有的文件名写入filename数组
    uint32_t file_crc=0;
    unsigned char *FULL_FILE_BUFFER = (unsigned char *) malloc(file_size);//算CRC32，先把整个文件读进来
    fseek(file, 0, SEEK_SET);
    fread(FULL_FILE_BUFFER, sizeof(unsigned char), file_size, file);
    file_crc=crc32(FULL_FILE_BUFFER, file_size);
    fseek(file, 0, SEEK_SET);
    system("mkdir steg");
    for(i=0;i<bmp_number;i++) {
        bmp_img img;
        enum bmp_error error = bmp_img_read(&img, filename[i]);
        if (error != BMP_OK) {
            printf("Error reading BMP file: %d\n", error);
            return 1;
        }
        STEG_HEADER* steg_header=create_steg_header(0, bmp_number, i+1, file_size, each_file_size[i],file_crc);//如果是第一个图片就是0
        unsigned char *raw_buffer = (unsigned char *) malloc(each_file_size[i]);
        fread(raw_buffer, sizeof(unsigned char), each_file_size[i], file);
        error = lsb_hide_binary(&img, each_file_size[i], raw_buffer, steg_header);
        chdir("steg");
        error = bmp_img_write(&img, filename[i]);
        if (error != BMP_OK) {
            printf("Error writing BMP file: %d\n", error);
            return 1;
        }
        bmp_img_free(&img);
        printf("LSB index: %d hiding completed.\n",i);
        chdir("..");
    }
    fclose(file);
    return 0;
}
// 文件结构：MBMPSTEG [软件版本信息3bytes]
// [文件加密标志位1bytes(使用最低位)](保留)
// [全部隐写的图片数量2bytes]
// [多图片隐写的图片序号2bytes]
// [隐写的文件总大小4bytes]
// [本文件中隐写的原始数据文件大小4bytes]
// [隐写的文件CRC32校验值]
// [本文件中隐写的原始数据CRC32校验值](保留)
// [隐写数据开始]

//STEG_HEADER create_steg_header(char enc, short all_bmp_number, short this_bmp_number, int all_steg_file_size,
//                               int this_steg_file_size, char all_steg_file_crc32[], char this_steg_file_crc32[])

STEG_HEADER* create_steg_header(char enc, short all_bmp_number, short this_bmp_number, int all_steg_file_size,
                               int this_steg_file_size,uint32_t all_steg_file_crc32) {
    STEG_HEADER *each_steg_header = (STEG_HEADER *) malloc(sizeof(STEG_HEADER));
    char software_name[] = "MBMPSTEG";
    memcpy(each_steg_header->SOFTWARE_NAME, software_name, sizeof(each_steg_header->SOFTWARE_NAME));
    char software_ver[] = "1.0";
    memcpy(each_steg_header->SOFTWARE_VER, software_ver, sizeof(each_steg_header->SOFTWARE_VER));
    each_steg_header->ENC_FLAG = enc;
    each_steg_header->ALL_BMP_NUMBER = all_bmp_number;
    each_steg_header->THIS_BMP_NUMBER = this_bmp_number;
    each_steg_header->ALL_STEG_FILE_SIZE = all_steg_file_size;
    each_steg_header->THIS_STEG_FILE_SIZE = this_steg_file_size;
    each_steg_header->ALL_STEG_FILE_CRC32 = all_steg_file_crc32;
    each_steg_header->THIS_STEG_FILE_CRC32 = 0;
//    memcpy(each_steg_header->ALL_STEG_FILE_CRC32, all_steg_file_crc32, sizeof(char) * 4);
//    memcpy(each_steg_header->THIS_STEG_FILE_CRC32, this_steg_file_crc32, sizeof(char) * 4);
    return each_steg_header;
}

int getNodeLength(FILENAMENODE *head) {
    int count = 0;
    FILENAMENODE *current = head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}