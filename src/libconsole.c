//
// Created by gfdfdxc.
//

#include "libconsole.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "Windows.h"

void printConsoleSelectList(char *format, int *counter, int selected, ...) {
    HANDLE windowHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (*counter == selected) {
        SetConsoleTextAttribute(windowHandle, 0x70);
    }

    va_list args;
    va_start(args, selected);
    // 不固定参数通过va_list保存，使用vprintf输出，可更好地封装print函数
    vprintf(format, args);
    va_end(args);

    SetConsoleTextAttribute(windowHandle, 0x07);
    (*counter)++;
}