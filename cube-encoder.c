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
    int i;

    // "Parse" arguments
    if (argc != 3) {
        printf("Usage: %s inputfile outputfile\n", argv[0]);
        return 1;
    }
    infilename = argv[1];
    outfilename = argv[2];


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
    uint8_t *data = malloc(outfilesize);
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
    uint8_t pad = outfilesize - infilesize;
    for (i = infilesize; i < outfilesize; i++) {
        data[i] = pad;
    }

    // Encrypt data
    blowfish_key_schedule((uint8_t*) userkey, &key, strlen(userkey));
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
