#ifndef AES_ENCODE_H
#define AES_ENCODE_H
#include "aes_basic.h"
void aes_encode_CBC(uint8_t *buf,const uint8_t *key,uint8_t *iv,size_t len);

#endif