#include "tabulation.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Total size should be cu^{1/c}. c = 8, u = 2^64, so cu^{1/c} ~
// 8*2^8*sizeof(int) -> 2^13 bytes -> 8KB.


// initialize hashFuncTable
// rand should be between 0 to 2^hashLength-1
// TODO: use a better random function


table_t* hashFuncTable_initialize(int hashLength) {
  table_t* hashFuncTable = malloc(NUM_CHUNKS * HASH_TABLE_SIZE * sizeof(table_t));
  for (int i = 0; i < NUM_CHUNKS; ++i) {
    for (int j = 0; j < HASH_TABLE_SIZE; ++j) {
      hashFuncTable[i * HASH_TABLE_SIZE + j] = randLim((1 << hashLength) - 1);
    }
  }
  return hashFuncTable;
}

// index from the front with chunk_index, starting at 0
// we break x into x_0|x_1|x_2|...|x_k-1
// assume 64 (size of long) is divisible by char size
table_t extractChunk(elt_t key, int chunkIndex) {
  int startIndex = CHUNK_SIZE * chunkIndex;
  return (key << startIndex) >> (KEY_LENGTH - CHUNK_SIZE);
}

// key is what we want to hash.
table_t tabulationHash(table_t* hashFuncTable, elt_t key) {
  table_t hashedKey = 0;
  for (int i = 0; i < NUM_CHUNKS; i++) {
    hashedKey ^= getHashChunk(hashFuncTable, extractChunk(key, i), i);
  }
  return hashedKey;
}

// hash the chunk using the table of hash functions
table_t getHashChunk(table_t* hashFuncTable, table_t chunkValue, int chunkIndex) {
  return hashFuncTable[chunkIndex * HASH_TABLE_SIZE + chunkValue];
}
