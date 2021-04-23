#ifndef linearprobe_H
#define linearprobe_H

#include "stdbool.h"
#include <stdlib.h>

// probeArray indexed by hash(key) 
typedef struct {
    unsigned long key;
    bool exist;
} data;

// stores the pointer to start 
typedef struct {
    data* start;
    int size;
    int collisions;
} probeArray;

// void probeArray_delete(probeArray arr) {
//     free((void*)(arr.start));
// }

probeArray probeArray_initialize(int tableSize);

void probeArray_insert(probeArray arr, long element);
bool probeArray_query(probeArray arr, long element);

#endif