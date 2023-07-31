//
// Created by gfdfdxc.
//

#include "error.h"
#include <stdio.h>
void checkErrorCode(int flag){
    switch (flag)
    {
    case -1:
        puts("\033[31mError Code 0xff: Not a Valid Input\033[0m");//可以加颜色
        break;
    case 0:
        puts("\033[32mSuccess!\033[0m");//可以加颜色
        break;
    case 0x1:
        puts("Error Code 0x1: Cannot open given Folder.");
        break;
    case 0x2:
        puts("Error Code 0x2: Insufficient BMP lsb bits.");//写入时文件太大图片太小
        break;
    case 0x3:
        puts("Error Code 0x3: Noting to delete");//剩余一张图片的时候还要delete
        break;
    case 0x4:
        puts("Error Code 0x4: Not a bmp file.");
        break;
    case 0x5:
        puts("Error Code 0x5: Cannot read file.");
        break;
    case 0x6:
        puts("Error Code 0x5: Cannot write file.");
        break;
    case 0x7:
        puts("\033[31mError Code 0x7: Manually canceled.\033[0m");
        break;
    case 0x8:
        puts("\033[31mError Code 0x8: Error allocating memory for binary file.\033[0m");
        break;
    case 0x9:
        puts("\033[31mError Code 0x9: This BMP file is not created by this software.\033[0m");
        break;
    case 0x10:
        puts("\033[31mError Code 0x10: CRC32 Check Failed.\033[0m");
        break;
    case 0x99:
        puts("Error Code 0x99: Incorrect Extract Password");
        break;
    default:
        break;
    }
}