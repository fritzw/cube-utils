#include <stdint.h>

typedef struct {
    uint32_t p[18];
    uint32_t s[4][256];
} BLOWFISH_KEY;


void blowfish_encrypt(uint8_t in[], uint8_t out[], BLOWFISH_KEY *keystruct, int compatibility_mode);
void blowfish_decrypt(uint8_t in[], uint8_t out[], BLOWFISH_KEY *keystruct, int compatibility_mode);
void blowfish_key_schedule(uint8_t user_key[], BLOWFISH_KEY *keystruct, int len);
