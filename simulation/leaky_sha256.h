#ifndef OWNSHA256_LEAK_H
#define OWNSHA256_LEAK_H

#include <stdlib.h>
#include <string.h>
#include "leak.h"

//#define ROTR(x,n) ((x>>n)|(x<<(32-n)))
#define ROTR(x,n) leak_uint_or(leak_uint_rightshift(x,n), leak_uint_leftshift(x, leak_uint_minus(32,n)))

//#define SHR(x,n) (x>>n)
#define SHR(x,n) leak_uint_rightshift(x,n)

//#define sigma_0(x) (ROTR(x,7)^ROTR(x,18)^SHR(x,3))
#define sigma_0(x) leak_uint_xor(leak_uint_xor(ROTR(x,7), ROTR(x,18)),SHR(x,3))

//#define sigma_1(x) (ROTR(x,17)^ROTR(x,19)^SHR(x,10))
#define sigma_1(x) leak_uint_xor(leak_uint_xor(ROTR(x,17),ROTR(x,19)),SHR(x,10))

//#define Sigma_0(x) (ROTR(x,2)^ROTR(x,13)^ROTR(x,22))
#define Sigma_0(x) leak_uint_xor(leak_uint_xor(ROTR(x,2),ROTR(x,13)),ROTR(x,22))

//#define Sigma_1(x) (ROTR(x,6)^ROTR(x,11)^ROTR(x,25))
#define Sigma_1(x) leak_uint_xor(leak_uint_xor(ROTR(x,6),ROTR(x,11)), ROTR(x,25))

//#define Ch(x,y,z) ((x&y)^((~x)&z))
#define Ch(x,y,z) leak_uint_xor(leak_uint_and(x,y),leak_uint_and(leak_uint_not(x),z))

//#define Maj(x,y,z) ((x&y)^(x&z)^(y&z))
#define Maj(x,y,z) leak_uint_xor(leak_uint_xor(leak_uint_and(x,y), leak_uint_and(x,z)), leak_uint_and(y,z))





static unsigned int SHA256_IV[8] = {0x6a09e667UL, 0xbb67ae85UL, 0x3c6ef372UL, 0xa54ff53aUL,
                             0x510e527fUL, 0x9b05688cUL, 0x1f83d9abUL, 0x5be0cd19UL};






void ownsha256_compression_function(unsigned int hash[8], unsigned char message_block[64]);
void ownsha256(const unsigned char *message, size_t message_length,
  unsigned char *message_digest);

#endif
