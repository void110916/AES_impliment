#ifndef AES_DECODE_H
#define AES_DECODE_H
#include "aes_basic.h"
void aes_decode_CBC(uint8_t *buf,const uint8_t *key,uint8_t *iv,size_t len);

#endif