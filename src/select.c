// Created by gfdfdxc.

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
#include "info.h"

FILENAMENODE *selectBMP() {
    FILENAMENODE *fileList = getAllFilesInFolder(dirPath);
    FILENAMENODE *selectedList = NULL;
    while (1) {
        char *selectedFilename = printAndSelectFilelist(&fileList,selectedList);
        if (selectedFilename == NULL) break;
        if (strcmp(selectedFilename,"Exit")==0) return NULL;
        addToSelectedList(&selectedList, selectedFilename);
    }
    return selectedList;
}

char *printAndSelectFilelist(FILENAMENODE **filenameList,FILENAMENODE *selectedList) {
    int counter = 0, selected = 0, key, itemCount = 0;
    FILENAMENODE *current;
    Menu_Refresh:
    counter = 0;
    current = *filenameList;
    system("chcp 65001>nul & cls");

    puts("+================+");
    puts("|    Selected    |");
    puts("+================+");
    printSelectedItems(selectedList);
    puts("+================+");
    puts("|    Selecting   |");
    puts("+================+");
    printf("\n");
    while (current != NULL) {
        itemCount++;
        printConsoleSelectList("\t%s\n", &counter, selected, current->fileName);
        current = current->next;
    }
    if ((*filenameList) == NULL){
        printf("\tYou have selected all the files!\n");
        return NULL;
    }
    itemCount--;
    printf("\n");
    puts("<Enter>Select <Esc>Finish Selecting <Ctrl-C>Exit");
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
            current = *filenameList;//再次获取index对应的文件名
            for (int i = 0; i < selected; i++) {
                current = current->next;
            }
            printf("You selected %d, name: %s\n", selected, current->fileName);
            //delete this filename from filenameList
            if (selected == 0) {
                *filenameList = (*filenameList)->next;
            } else {
                FILENAMENODE *previous = *filenameList;
                for (int i = 0; i < selected - 1; i++) {
                    previous = previous->next;
                }
                previous->next = current->next;
            }
            return current->fileName;
        case 27: //ESC
            return NULL;
        case 3:// CtrlC
            return "Exit";
    }
    goto Menu_GetKey;
}

void addToSelectedList(FILENAMENODE **selectedList, char *filename) {
    FILENAMENODE *head = *selectedList;
    FILENAMENODE *current = NULL;
    if (head == NULL) {
        head = (FILENAMENODE *) malloc(sizeof(FILENAMENODE));
        head->fileName = (char *) malloc((strlen(filename) + 1) * sizeof(char));
        strcpy(head->fileName, filename);
        head->next = NULL;
        *selectedList = head;
    } else {
        current = head;
        // Find end of linked list
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = (FILENAMENODE *) malloc(sizeof(FILENAMENODE));
        current->next->fileName = (char *) malloc((strlen(filename) + 1) * sizeof(char));
        strcpy(current->next->fileName, filename);
        current->next->next = NULL;
    }
}

void printSelectedItems(FILENAMENODE *selectedList) {
    FILENAMENODE *current = selectedList;
    int counter = 0;
    while (current != NULL) {
        printf("index:%d, name: %s\n", counter, current->fileName);
        current = current->next;
        counter++;
    }
}
