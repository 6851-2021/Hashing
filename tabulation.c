#include "tabulation.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Total size should be cu^{1/c}. c = 8, u = 2^64, so cu^{1/c} ~
// 8*2^8*sizeof(int) -> 2^13 bytes -> 8KB.

int hashFuncTable[NUM_CHUNKS][HASH_TABLE_SIZE];

// initialize hashFuncTable
// rand should be between 0 to 2^hashLength-1
// TODO: use a better random function
void hashFuncTable_initialize(int hashLength) {
  for (int i = 0; i < NUM_CHUNKS; ++i) {
    for (int j = 0; j < HASH_TABLE_SIZE; ++j) {
      hashFuncTable[i][j] = randLim((1 << hashLength) - 1);
    }
  }
}

// index from the front with chunk_index, starting at 0
// we break x into x_0|x_1|x_2|...|x_k-1
// assume 64 (size of long) is divisible by char size
unsigned long extractChunk(unsigned long key, int chunkSize, int chunkIndex) {
  int startIndex = chunkSize * chunkIndex;
  return (key << startIndex) >> (KEY_LENGTH - chunkSize);
}

// key is what we want to hash.
unsigned long tabulationHash(unsigned long key, int numChunks, int chunkSize) {
  unsigned long hashedKey = 0;
  for (int i = 0; i < numChunks; i++) {
    hashedKey ^= hashChunk(extractChunk(key, chunkSize, i), i);
  }
  return hashedKey;
}

// hash the chunk using the table of hash functions
unsigned long hashChunk(unsigned long chunkValue, int chunkIndex) {
  return hashFuncTable[chunkIndex][chunkValue];
}
