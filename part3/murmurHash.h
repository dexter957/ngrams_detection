
#include <stdint.h>

unsigned int murmur2(const void* key, int length, const unsigned int seed);
uint32_t murmur3_32(const void* key, int length, const unsigned int seed);