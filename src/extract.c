//
// Created by gfdfdxc.
//

#include "extract.h"
#include "info.h"
#include <stdio.h>
#include <dirent.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "libconsole.h"
#include "common.h"
#include "libbmp.h"
#include "select.h"
#include "steg.h"
#include "crc32.h"
#include "main.h"

int mainExtract() {
    puts("+===================+");
    puts("|Extract Folder/File|");
    puts("+===================+");
    changeWorkingDir();
    FILENAMENODE *fileList = getAllFilesInFolder(dirPath);
    int allFilesCount = getNodeLength(fileList);

    char *filename[allFilesCount];//创建一个文件夹下带有所有bmp文件名的char*数组
    int i = 0;
    FILENAMENODE *tempfilenameList = fileList;
    while (tempfilenameList != NULL) {
        filename[i] = tempfilenameList->fileName;
        tempfilenameList = tempfilenameList->next;
        i++;
    }//把所有的文件名写入filename数组
    STEG_HEADER all_header[allFilesCount];//创建一个文件夹下所有bmp文件的steg_header结构体数组
    for (i = 0; i < allFilesCount; i++) {
        all_header[i] = get_steg_header(filename[i]);
    }//把所有的steg_header写入all_header数组
    //遍历所有all_header，找出不同的all_header[i].ALL_STEG_FILE_CRC32，并写入到一个新的链表(更新：不要链表了，直接数组)
    //    CRC32NODE *CRC32List = getCRC32List(all_header, allFilesCount);
    uint32_t CRC32List[allFilesCount - 1];
    for (i = 0; i < allFilesCount; i++) {
        CRC32List[i] = all_header[i].ALL_STEG_FILE_CRC32;
    }
    uint32_t aim_crc32 = selectCRC32(CRC32List,allFilesCount);
    if (aim_crc32 == -1) {
        return 0x7;
    }
    int ALL_BMP_NUMBER = 0, ALL_STEG_FILE_SIZE = 0;
    for (i = 0; i < allFilesCount; i++) { //先知道有几个对应的文件与总文件的大小
        if (all_header[i].ALL_STEG_FILE_CRC32 == aim_crc32) {
            ALL_BMP_NUMBER = all_header[i].ALL_BMP_NUMBER;
            ALL_STEG_FILE_SIZE = all_header[i].ALL_STEG_FILE_SIZE;
            break;
        }
    }
    int currentBMPIndex = 0;
    uint32_t currentBMPBufferPos = 0;
    unsigned char *buffer = (unsigned char *) malloc(ALL_STEG_FILE_SIZE);
    if (buffer == NULL) {
        return 0x8;
    }
    while (currentBMPIndex < ALL_BMP_NUMBER) {
        int partFileSize = 0;
        for (i = 0; i < allFilesCount; i++) { //找到对应的index，并写入buffer
            if (all_header[i].THIS_BMP_NUMBER-1 == currentBMPIndex) {
                partFileSize = all_header[i].THIS_STEG_FILE_SIZE;
                unsigned char *partBuffer = (unsigned char *) malloc(partFileSize);
                partBuffer = lsb_get_binary(filename[i], partFileSize + sizeof(STEG_HEADER));
                memcpy(buffer + currentBMPBufferPos, partBuffer + sizeof(STEG_HEADER), partFileSize);
                currentBMPBufferPos += partFileSize;
                currentBMPIndex++;
                free(partBuffer);
                break;
            }
        }
    }
    FILE *fp = fopen("extracted.bin", "wb");
    fwrite(buffer, sizeof(unsigned char), ALL_STEG_FILE_SIZE, fp);
    uint32_t calced_crc32 = crc32(buffer, ALL_STEG_FILE_SIZE);
    fclose(fp);
    if (calced_crc32 == aim_crc32) {
        printf("CRC32 Check Passed.\n");
        return 0;
    } else {
        printf("CRC32 Check Failed.\n");
        return 0x10;
    }

}

STEG_HEADER get_steg_header(char *filename) {
    FILE *fp = fopen(filename, "rb");
    STEG_HEADER steg_header;
    unsigned char *header_buffer = lsb_get_binary(filename, sizeof(STEG_HEADER));
    memcpy(&steg_header, header_buffer, sizeof(STEG_HEADER));
    fclose(fp);
    return steg_header;
}

uint32_t selectCRC32(const uint32_t CRC32List[],int allFilesCount) {
    int counter = 0, selected = 0, key, itemCount = 0;
    CRC32NODE *current;
    Menu_Refresh:
    counter = 0;
    system("chcp 65001>nul & cls");
    puts("+================+");
    puts("|    Selecting   |");
    puts("+================+");
    printf("\n");
    puts("\tNote: You can select any file with the same CRC32 value.");
    while (itemCount < allFilesCount) {
        printConsoleSelectList("\tIndex: %d, CRC32: 0x%08X\n", &counter, selected, itemCount, CRC32List[itemCount]);
        itemCount++;
    }
    printf("\n");
    puts("<Enter>Select <Ctrl-C>Exit");
    Menu_GetKey:
    key = _getch();
    switch (key) {
        case 224: //方向键总以224开头，参考 https://stackoverflow.com/questions/10463201/getch-and-arrow-codes
            key = _getch();
            switch (key) {
                case 80: // 下
                    if (selected < itemCount) selected++;
                    else selected = 0;
                    itemCount = 0;
                    goto Menu_Refresh;
                case 72: // 上
                    if (selected > 0) selected--;
                    else selected = itemCount;
                    itemCount = 0;
                    goto Menu_Refresh;
                default:
                    break;
            }
            break;
        case 13: //回车
            return CRC32List[selected];
        case 3:// CtrlC
            return -1;
    }
    goto Menu_GetKey;
}

unsigned char *lsb_get_binary(char *filename, size_t partFileSize) {// return *raw_buffer
    bmp_img img;
    enum bmp_error error = bmp_img_read(&img, filename);
    if (error != BMP_OK) {
        printf("Error reading BMP file: %d\n", error);
        return NULL;
    }
    printf("Now I will get %zu bytes from %s\n", partFileSize, filename);
    unsigned char *buffer = (unsigned char *) malloc(partFileSize);
    if (buffer == NULL) {
        printf("Error allocating memory for binary file.\n");
        return NULL;
    }
    memset(buffer, 0, partFileSize);


    size_t bit_index = 0;
    for (size_t y = 0; y < img.img_header.biHeight; y++) {
        for (size_t x = 0; x < img.img_header.biWidth; x++) {
            bmp_pixel *pixel = &img.img_pixels[y][x];

            // 隐藏红色通道
            if (bit_index < partFileSize * 8) {
                buffer[bit_index >> 3] |= ((pixel->red & 0x01) << (7 - (bit_index & 7)));
                bit_index++;
            }
            // 隐藏绿色通道
            if (bit_index < partFileSize * 8) {
                buffer[bit_index >> 3] |= ((pixel->green & 0x01) << (7 - (bit_index & 7)));
                bit_index++;
            }
            // 隐藏蓝色通道
            if (bit_index < partFileSize * 8) {
                buffer[bit_index >> 3] |= ((pixel->blue & 0x01) << (7 - (bit_index & 7)));
                bit_index++;
            }
            if (bit_index >= partFileSize * 8) {
                break;
            }
            if (bit_index >= partFileSize * 8) {
                break;
            }
        }
    }
    return buffer;
}