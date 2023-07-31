//
// Created by gfdfdxc.
//

#ifndef FINAL_CRC32_H
#define FINAL_CRC32_H
#include <stdint.h>
uint32_t crc32(const unsigned char* buffer, size_t length);
typedef struct CRC32Node{
    uint32_t crc32_value;
    struct CRC32Node *next;
}CRC32NODE;
#endif //FINAL_CRC32_H
