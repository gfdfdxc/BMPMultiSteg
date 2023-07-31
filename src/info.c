//
// Created by gfdfdxc.
//

#include "info.h"
#include <stdio.h>
#include <dirent.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "libconsole.h"
#include "common.h"
#include "extract.h"
#include "libbmp.h"
// 读取文件夹下所有文件 https://blog.51cto.com/dgwblog/5556515

int mainInfo() {
    puts("+================+");
    puts("|File/Folder Info|");
    puts("+================+");
    FILENAMENODE *fileList = getAllFilesInFolder(dirPath);
    char *selectedFilename = printAllFilesinFolder(fileList);
    return print_bmp_info(selectedFilename);
}

char *printAllFilesinFolder(FILENAMENODE *filenameList) {

    int counter = 0, selected = 0, key, itemCount = 0;
    FILENAMENODE *current;
    Menu_Refresh:
    counter = 0;
    current = filenameList;

    system("chcp 65001>nul & cls");
    puts("+================+");
    puts("|File/Folder Info|");
    puts("+================+");
    printf("\n");
    while (current != NULL) {
        itemCount++;
        printConsoleSelectList("\t%s\n", &counter, selected, current->fileName);
        current = current->next;
    }
    itemCount--;
    printf("\n");
    puts("<Enter>View file info <Esc>Exit");
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
            current = filenameList;//再次获取index对应的文件名
            for (int i = 0; i < selected; i++) {
                current = current->next;
            }
            printf("You selected %d, name: %s\n", selected, current->fileName);
            return current->fileName;
        case 27: //ESC
            return NULL;
    }
    goto Menu_GetKey;
}


FILENAMENODE *getAllFilesInFolder(char *dirPath) {
    DIR *dir;
    struct dirent *ptr;
    dir = opendir(dirPath);
    FILENAMENODE *head = NULL;
    FILENAMENODE *current = NULL;
    while ((ptr = readdir(dir)) != NULL) {
        if (!(endsWith(ptr->d_name, ".bmp") || endsWith(ptr->d_name, ".BMP")))continue;//筛选以bmp结尾的文件
        FILENAMENODE *newNode = (FILENAMENODE *) malloc(sizeof(FILENAMENODE));
        newNode->fileName = (char *) malloc((strlen(ptr->d_name) + 1) * sizeof(char));
        strcpy(newNode->fileName, ptr->d_name);
        newNode->next = NULL;
        if (head == NULL) {
            head = newNode;
            current = newNode;
        } else {
            current->next = newNode;
            current = current->next;
        }
    }
    closedir(dir);
//    freeLinkedList(head);
    return head;
}

void freeLinkedList(FILENAMENODE *head) {
    FILENAMENODE *current = head;
    while (current != NULL) {
        FILENAMENODE *temp = current;
        current = current->next;
        free(temp->fileName);
        free(temp);
    }
}

int endsWith(const char *str, const char *suffix) {
    int strLength = strlen(str);
    int suffixLength = strlen(suffix);

    if (strLength < suffixLength) {
        return 0;
    }

    const char *strSuffix = str + (strLength - suffixLength);
    return strcmp(strSuffix, suffix) == 0;
}

int print_bmp_info(const char *filename) {
    bmp_img img;
    enum bmp_error error = bmp_img_read(&img, filename);

    if (error != BMP_OK) {
        printf("Error reading BMP file: %d\n", error);
        return 1;
    }
    printf("BMP Header Information:\n");
    printf("Width: %d pixels\n", img.img_header.biWidth);
    printf("Height: %d pixels\n", img.img_header.biHeight);
    printf("Bit Count: %d\n", img.img_header.biBitCount);
    printf("Image Size: %d bytes\n", img.img_header.bfSize);
//    printf("Compression: %d\n", img.img_header.biCompression);
    bmp_img_free(&img);
    STEG_HEADER my_header=get_steg_header(filename);
    if (memcmp(my_header.SOFTWARE_NAME,"MBMPSTEG",8)!=0){
        printf("This file is not created by this software!\n");
        return 0x9;
    }
    printf("\n");
    printf("======Steg Header Information======\n");
    printf("Software Version: %s\n", my_header.SOFTWARE_VER);
    printf("Encrypt flag (Not enabled): %d\n", my_header.ENC_FLAG);
    printf("ALL BMP number of this steged file: %d\n", my_header.ALL_BMP_NUMBER);
    printf("The file index of this series: %d\n", my_header.THIS_BMP_NUMBER);
    printf("The full steged file is: %d Bytes\n", my_header.ALL_STEG_FILE_SIZE);
    printf("The steged data in this file is: %d Bytes\n", my_header.THIS_STEG_FILE_SIZE);
    printf("The CRC32 checksum of steged file is: 0x%08X\n", my_header.ALL_STEG_FILE_CRC32);
    return 0;
}