#ifndef tabulation_H
#define tabulation_H

#include <stdlib.h>

#include "utils.h"

// return a random number between 0 and limit inclusive.
static int randLim(int limit) {
  int divisor = RAND_MAX / (limit + 1);
  int retval;

  do {
    retval = rand() / divisor;
  } while (retval > limit);

  return retval;
}

table_t* hashFuncTable_initialize(int hashLength);
table_t extractChunk(elt_t key, int chunkIndex);
table_t tabulationHash(table_t* hashFuncTable, elt_t key);
table_t getHashChunk(table_t* hashFuncTable, table_t chunkValue,
                     int chunkIndex);

#endif