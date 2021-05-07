#ifndef cuckoo_H
#define cuckoo_H

#include "utils.h"
#include <stdbool.h>
#include <stdlib.h>

// stores the pointer to start 
typedef struct {
    data* start1;
    data* start2;
    table_t* table1;
    table_t* table2;
    int size;
    int collisions;
    int rebuilds;
} cuckooArray;

cuckooArray* cuckooArray_make(int tableSize, elt_t* elements, int numElements);
bool cuckooArray_query(cuckooArray* arr, elt_t element);
void cuckooArray_delete(cuckooArray* arr);

#endif