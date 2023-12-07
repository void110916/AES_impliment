#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "aes_encode.h"
int main(int argc, char *argv[]){
    if(argc<4)
    {
        printf("Not enough argument!!!\n");
        return EXIT_FAILURE;
    }
    
    const char *keytext=argv[2];
    switch (strlen(keytext))
    {
    case 128:
        
        break;
    case 256:
        break;
    default:
        printf("Key length uncorrect(need 16 words(128-bit) or 32 words(256-bit)\n");
        return EXIT_FAILURE;
        break;
    }
    const char *filename=argv[1];
    FILE *ifile=fopen(filename,"r");
    if(ifile==NULL)
    {
        printf("No file exist.\n");
        return EXIT_FAILURE;
    }
    stat_t stat;
    int len=0xff;
    char *ofilename=malloc(strlen(filename)+strlen("_aes"));
    strcpy(ofilename,filename);
    strcat(ofilename,"_aes");
    FILE *ofile=fopen(ofilename,"w");
    while (1)
    {
        len=fread(&stat,1,16,ifile);
        if(feof(ifile))
            break;
        aes_encode_CBC(stat,keytext,keytext,len);
        fwrite(stat,1,sizeof(stat),ofile);
    }
    if(len==16){
        aes_encode_CBC(stat,keytext,keytext,len);
        memset(stat,16,sizeof(stat));
        aes_encode_CBC(stat,keytext,keytext,len);
    }
    else
    {
        padding(stat,16-len);
        aes_encode_CBC(stat,keytext,keytext,len);
    }
    fclose(ifile);
    fclose(ofile);
    
    return EXIT_SUCCESS;
}