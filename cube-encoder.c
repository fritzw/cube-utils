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
    char *extension = ".cubepro";
    BLOWFISH_KEY key;
    char *infilename, *outfilename;
    int i;

    /* TODO:
    Implement flag to enable CubeX encoding?
    Implement decoding of .cubepro/.cubex files?
    Implement legacy mode for compatibility with CodeX:
    argv[0] [CUBEPRO | CUBEX] [ENCODE | DECODE | RECODE] inputfile outputfile
    */
    
    if (strncmp(argv[0] + strlen(argv[0]) - 13, "cubex-encoder", 13) == 0) {
        userkey = cubex_key;
        extension = ".cubex";
    }

    // "Parse" arguments
    if (argc == 2) {
        infilename = argv[1];
        size_t inlength = strlen(infilename);
        outfilename = malloc(inlength + 10);
        if (outfilename == NULL) {
            perror("Unable to allocate memory for output file name");
            return 4;
        }
        strncpy(outfilename, infilename, inlength);
        if (strncmp(outfilename + inlength - 4, ".bfb", 4) == 0) {
            strcpy(outfilename + inlength - 4, extension);
        } else {
            strcpy(outfilename + inlength, extension);
        }
    } else if (argc == 3) {
        infilename = argv[1];
        outfilename = argv[2];
    } else {
        printf("Usage:\n"
               "    %s inputfile [outputfile]\n"
               "    Default outputfile is inputfile minus the .bfb extension plus %s\n",
               argv[0], extension);
        return 1;
    }


    // Load input file
    FILE *infile = fopen(infilename, "r");
    if (infile == NULL) {
        perror("Unable to open input file");
        return 2;
    }
    size_t infilesize = fsize(infile);
    if (infilesize < 0) {
        perror("Unable to determine size of input file");
        return 3;
    }
    size_t outfilesize = ((infilesize + 8) / 8) * 8;
    BYTE *data = malloc(outfilesize);
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

    // Pad input data
    BYTE pad = outfilesize - infilesize;
    for (i = infilesize; i < outfilesize; i++) {
        data[i] = pad;
    }

    // Encrypt data
    blowfish_key_setup((BYTE*) userkey, &key, strlen(userkey));
    for (i = 0; i < outfilesize; i += 8) {
        blowfish_encrypt(&data[i], &data[i], &key, 1);
    }


    // Save data to output file
    FILE *outfile = fopen(outfilename, "w+");
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
