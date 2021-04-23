#ifndef tabulation_H
#define tabulation_H

#include <stdlib.h>

static const int KEY_LENGTH = 64;
static const int CHUNK_SIZE = 8;
static const int NUM_CHUNKS = ((KEY_LENGTH - 1) / CHUNK_SIZE) + 1;
static const int HASH_TABLE_SIZE = 1 << CHUNK_SIZE;  // 2^CHUNK_SIZE

// return a random number between 0 and limit inclusive.
static int randLim(int limit) {
  int divisor = RAND_MAX / (limit + 1);
  int retval;

  do {
    retval = rand() / divisor;
  } while (retval > limit);

  return retval;
}

void hashFuncTable_initialize(int hashLength);
unsigned long extractChunk(unsigned long key, int chunkSize, int chunkIndex);
unsigned long tabulationHash(unsigned long key, int chunkSize, int numChunks);
unsigned long hashChunk(unsigned long chunkValue, int chunkIndex);

#endif