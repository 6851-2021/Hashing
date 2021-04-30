#ifndef linearprobe_H
#define linearprobe_H

#include "utils.h"
#include <stdlib.h>

// probeArray indexed by hash(key) 
// stores the pointer to start 
typedef struct {
    data* start;
    int size;
    int collisions;
    table_t* hashTable;
} probeArray;

void probeArray_delete(probeArray* arr);

probeArray probeArray_initialize(int tableSize);

void probeArray_insert(probeArray * arr, elt_t element);
bool probeArray_query(probeArray * arr, elt_t element);

#endif