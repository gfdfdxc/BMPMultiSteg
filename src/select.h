//
// Created by gfdfdxc.
//

#ifndef FINAL_SELECT_H
#define FINAL_SELECT_H
#include "info.h"
#include <stdio.h>
#include <dirent.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "libconsole.h"
#include "common.h"
#include "libbmp.h"
FILENAMENODE *selectBMP();
char *printAndSelectFilelist(FILENAMENODE **filenameList,FILENAMENODE *selectedList);
void addToSelectedList(FILENAMENODE **selectedList, char *filename);
void printSelectedItems(FILENAMENODE *selectedList);
#endif //FINAL_SELECT_H
