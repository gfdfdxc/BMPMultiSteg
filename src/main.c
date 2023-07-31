#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "error.h"
#include "info.h"
#include "steg.h"
#include "extract.h"
#include "common.h"
#include <unistd.h>

char dirPath[240];

void welcome() {
    system("chcp 65001");
    system("title BMPMultiSteg v1.0");
    system("CLS");
    puts("+=======================+");//ASCII大字符
    puts("|Welcome to BMPMultiSteg|");
    puts("+=======================+");
    printf("Please input the path of the working folder:\n");
    scanf("%s", dirPath);
    chdir(dirPath);
    system("CLS");
}

int changeWorkingDir() {
    fflush(stdin);
    printf("Please input the NEW path of the working folder, enter 'n' to remain current path:\n");
    char temp[240];
    scanf("%s", temp);
    if (strcmp(temp, "n") == 0) return 0;
    chdir(temp);
    return 0;
}

int main() {
    welcome();
    char select = 0;
    int error;
    while (1) {
        printf("Working Folder: %s\n\n", dirPath);
        printf("<1>File Info <2>Steg <3>Extract <4>Change Working Dir <Ctrl-C>Exit\n");
        select = getch();//特点：使用了getch函数，无需用户输入键盘回车，提高用户体验
        system("CLS");
        switch (select) {
            case '1':
                error = mainInfo();
                break;
            case '2':
                error = mainSteg();
                break;
            case '3':
                error = mainExtract();
                break;
            case '4':
                error = changeWorkingDir();
                break;
            case 3:// CtrlC
                printf("Bye\n");
                return 0;
            default:
                system("CLS");//功能：自动识别异常输入
                error = -1;
                printf("You typed %c(ASCII:%d), it is not a valid value.\n", select, select);
                break;
        }
        checkErrorCode(error);//因为有多种错误所以统一用一个函数来处理

    }
}
