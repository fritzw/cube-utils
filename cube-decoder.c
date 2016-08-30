#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "blowfish.h"


long int fsize(FILE *f) {
    long int original, size;
    original = ftell(f);
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, original, SEEK_SET);
    return size;
}


int main(int argc, char **argv) {
    char cubepro_key[] = "221BBakerMycroft";
    char cubex_key[] = "kWd$qG*25Xmgf-Sg";
    char *userkey = cubepro_key;
    BLOWFISH_KEY key;
    char *infilename, *outfilename;
    size_t outfilesize;
    int i;

    /* TODO:
    Implement flag to enable CubeX encoding?
    Implement decoding of .cubepro/.cubex files?
    Implement legacy mode for compatibility with CodeX:
    argv[0] [CUBEPRO | CUBEX] [ENCODE | DECODE | RECODE] inputfile outputfile
    */

    // "Parse" arguments
    if (argc == 4 && strcmp("-x", argv[1]) == 0) {
        userkey = cubex_key;
        infilename = argv[2];
        outfilename = argv[3];
    } else if (argc == 3) {
        infilename = argv[1];
        outfilename = argv[2];
    } else {
        printf("Usage:\n"
               "    %s [-x] inputfile outputfile\n"
               "    Normally, the inputfile will be decoded as a .cubepro file.\n"
               "    If the file name ends in .cubex or the -x parameter is present,i\n"
               "    it will be decoded as a .cubex file instead.\n",
               argv[0]);
        return 1;
    }

    if (strcmp(infilename + strlen(infilename) - 6, ".cubex") == 0) {
        userkey = cubex_key;
    }

    // Load input file
    FILE *infile = fopen(infilename, "rb");
    if (infile == NULL) {
        perror("Unable to open input file");
        return 2;
    }
    size_t infilesize = fsize(infile);
    if (infilesize < 0) {
        perror("Unable to determine size of input file");
        return 3;
    }
    BYTE *data = malloc(infilesize);
    if (data == NULL) {
        perror("Unable to allocate memory for input file");
        return 4;
    }
    size_t readcount = fread(data, 1, infilesize, infile);
    if (readcount != infilesize) {
        printf("Unable to read the whole input file: %zd != %zd\n", readcount, infilesize);
        return 5;
    }
    fclose(infile);

    // Decrypt data
    blowfish_key_setup((BYTE*) userkey, &key, strlen(userkey));
    for (i = 0; i < infilesize; i += 8) {
        blowfish_decrypt(&data[i], &data[i], &key, 1);
    }

    // Remove padding
    BYTE pad = data[infilesize - 1];
    if (pad > 8) {
        printf("Decoding error: Invalid padding. Make sure that this is a valid encoded file.\n");
        //return 8;
    }
    outfilesize = infilesize - pad;

    // Save data to output file
    FILE *outfile = fopen(outfilename, "wb+");
    if (outfile == NULL) {
        perror("Unable to open output file");
        return 6;
    }
    size_t writecount = fwrite(data, 1, outfilesize, outfile);
    if (writecount != outfilesize) {
        printf("Unable to write the whole output file: %zd != %zd\n", writecount, outfilesize);
        return 7;
    }
    fclose(outfile);

    return 0;
}
