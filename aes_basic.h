#ifndef AES_BASIC_H
#define AES_BASIC_H

#include <stdint.h>
#include <string.h>

#if defined(AES256) && (AES256 == 1)
    int Nk= 8;
    int Nr= 14;
#elif defined(AES192) && (AES192 == 1)
    int Nk= 6;
    int Nr= 12;
#else
    int Nk= 4;        // The number of 32 bit words in a key.
    int Nr= 10;       // The number of rounds in AES Cipher.
#endif

typedef uint8_t stat_t[4*4];

void block2stat(uint8_t *block, stat_t *stat);

void stat2block(stat_t *stat, uint8_t *block);

void addRoundKey(stat_t *stat,const uint8_t* roundkey,uint8_t round);

void keyExpain(const uint8_t *key,uint8_t *roundKey);

void xor(uint8_t *buf,uint8_t *iv);

#endif  // AES_BASIC_H