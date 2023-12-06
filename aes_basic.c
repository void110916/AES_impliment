#include "aes_basic.h"

extern const uint8_t subTable [16*16];

/**
 * @brief transpose block(byte array) to stat
 * @details need to transpose block matrix(row to col,col to row)
 * @param block 
 * @param stat 
 */
void block2stat(uint8_t *block, stat_t *stat){
    // https://stackoverflow.com/questions/16737298/what-is-the-fastest-way-to-transpose-a-matrix-in-c
    #pragma omp parallel for
    for(int n = 0; n<4*4; n++) {
        int i = n/4;
        int j = n%4;
        (*stat)[i*4+j] = block[4*j + i];
    }
}

/**
 * @brief transpose stat to block(byte array)
 * @details need to transpose block matrix(row to col,col to row)
 * @param stat 
 * @param block 
 */
void stat2block(stat_t *stat, uint8_t *block){
    // https://stackoverflow.com/questions/16737298/what-is-the-fastest-way-to-transpose-a-matrix-in-c
    #pragma omp parallel for
    for(int n = 0; n<4*4; n++) {
        int i = n/4;
        int j = n%4;
        block[4*j + i]=(*stat)[i*4+j];
    }
}

void addRoundKey(stat_t *stat,const uint8_t* roundkey,uint8_t round){
    #pragma omp parallel for
    for (int i = 0; i < 4*4; i++)
    {
        (*stat)[i]^=roundkey[i+round*4*4];
    }
}

static const uint8_t Rcon[11] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

void keyExpain(const uint8_t *key,uint8_t *roundKey){
    uint8_t tempa[4];
    int j,k;
    for (int i = 0; i < 4; i++)
    {
        roundKey[(i * 4) + 0] = key[(i * 4) + 0];
        roundKey[(i * 4) + 1] = key[(i * 4) + 1];
        roundKey[(i * 4) + 2] = key[(i * 4) + 2];
        roundKey[(i * 4) + 3] = key[(i * 4) + 3];
    }
    
    for (int i = 4; i < 44; i++)
    {
        {
            k = (i - 1) * 4;
            tempa[0]=roundKey[k + 0];
            tempa[1]=roundKey[k + 1];
            tempa[2]=roundKey[k + 2];
            tempa[3]=roundKey[k + 3];
        }

        if (i % 4 == 0)
        {
        // This function shifts the 4 bytes in a word to the left once.
        // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

        // Function RotWord()
        {
            uint32_t *row;
            // const uint8_t u8tmp = tempa[0];
            // tempa[0] = tempa[1];
            // tempa[1] = tempa[2];
            // tempa[2] = tempa[3];
            // tempa[3] = u8tmp;

            row=tempa;
            (*row)=(*row)<<(1<<3)|(*row)>>((4-1)<<3);
        }

        // SubWord() is a function that takes a four-byte input word and 
        // applies the S-box to each of the four bytes to produce an output word.

        // Function Subword()
        {
            tempa[0] = subTable[tempa[0]];
            tempa[1] = subTable[tempa[1]];
            tempa[2] = subTable[tempa[2]];
            tempa[3] = subTable[tempa[3]];
        }

        tempa[0] = tempa[0] ^ Rcon[i/4];
        }
#if defined(AES256) && (AES256 == 1)
        if (i % Nk == 4)
        {
        // Function Subword()
        {
            tempa[0] = subTable[tempa[0]];
            tempa[1] = subTable[tempa[1]];
            tempa[2] = subTable[tempa[2]];
            tempa[3] = subTable[tempa[3]];
        }
        }
#endif
        j = i * 4; k=(i - 4) * 4;
        roundKey[j + 0] = roundKey[k + 0] ^ tempa[0];
        roundKey[j + 1] = roundKey[k + 1] ^ tempa[1];
        roundKey[j + 2] = roundKey[k + 2] ^ tempa[2];
        roundKey[j + 3] = roundKey[k + 3] ^ tempa[3];
    }
    
    
}

void xor(uint8_t *buf,uint8_t *iv){
    uint32_t *buf_ptr=buf,*iv_ptr=iv; 
    for (int i = 0; i < 4; i++)
    {
        *(buf_ptr++)^=*(iv++);
    }
}