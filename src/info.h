//
// Created by gfdfdxc.
//

#ifndef FINAL_INFO_H
#define FINAL_INFO_H

#include "libbmp.h"

typedef struct filenameNode {
    char *fileName;
    struct filenameNode *next;
}FILENAMENODE;
int mainInfo();
char* printAllFilesinFolder(struct filenameNode *filenameList);
int endsWith(const char* str, const char* suffix);
struct filenameNode *getAllFilesInFolder();
int print_bmp_info(const char* filename);
void freeLinkedList(struct filenameNode *head);
#endif //FINAL_INFO_H
