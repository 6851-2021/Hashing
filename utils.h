#ifndef util_H
#define util_H

#include "stdbool.h"

static const int KEY_LENGTH = 64;
static const int CHUNK_SIZE = 8;
static const int NUM_CHUNKS = ((KEY_LENGTH - 1) / CHUNK_SIZE) + 1;
static const int HASH_TABLE_SIZE = 1 << CHUNK_SIZE;  // 2^CHUNK_SIZE


typedef int table_t;
typedef unsigned long elt_t;

typedef struct {
    elt_t key;
    bool exist;
} data;

#endif
