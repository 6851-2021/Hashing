#include "tabulation.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Total size should be cu^{1/c}. c = 8, u = 2^64, so cu^{1/c} ~
// 8*2^8*sizeof(int) -> 2^13 bytes -> 8KB.

/* initialize hashFuncTable
 * @param hashLength - # bits of result from hash table
 */
// TODO: use a better random function
table_t* hashFuncTable_initialize(int hashLength) {
  table_t* hashFuncTable =
      malloc(NUM_CHUNKS * HASH_TABLE_SIZE * sizeof(table_t));
  for (int i = 0; i < NUM_CHUNKS; ++i) {
    for (int j = 0; j < HASH_TABLE_SIZE; ++j) {
      hashFuncTable[i * HASH_TABLE_SIZE + j] = randLim((1 << hashLength) - 1);
    }
  }
  return hashFuncTable;
}

/*
 *  Extract a CHUNK_SIZE-bit chunk of an element/key
 *  Chunks are indexed from the front, starting at 0
 *  we break the key into x_0|x_1|x_2|...|x_k-1
 *  assume 64 (size of long) is divisible by char size
 *  @param key the element we are getting a part of
 *  @param chunkIndex the index of the chunk we are getting
 */
table_t extractChunk(elt_t key, int chunkIndex) {
  int startIndex = CHUNK_SIZE * chunkIndex;
  return (key << startIndex) >> (KEY_LENGTH - CHUNK_SIZE);
}

/*
 *  Return the result of a hash function associated with a table_t* object on a
 * key
 *  @hashFuncTable the table that defines the hash function
 *  @key the input to the hash function
 */
table_t tabulationHash(table_t* hashFuncTable, elt_t key) {
  table_t hashedKey = 0;
  for (int i = 0; i < NUM_CHUNKS; i++) {
    hashedKey ^= getHashChunk(hashFuncTable, extractChunk(key, i), i);
  }
  return hashedKey;
}

/*
 *  Return the result of hashing one chunk using the hash function
 *  @hashFuncTable the table that defines the hash function
 *  @chunkValue the value of the chunk extracted from a key being hashed
 *  @chunkIndex the index of the chunk extracted from the key
 */
table_t getHashChunk(table_t* hashFuncTable, table_t chunkValue,
                     int chunkIndex) {
  return hashFuncTable[chunkIndex * HASH_TABLE_SIZE + chunkValue];
}
