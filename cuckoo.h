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
} cuckooArray;


cuckooArray cuckooArray_initialize(int tableSize);

bool cuckooArray_insertAll(cuckooArray* arr, elt_t* elements, int numElements);
bool cuckooArray_insert(cuckooArray* arr, elt_t element, bool* isOccupied1, bool* isOccupied2);
bool cuckooArray_kick(cuckooArray* arr, int element_index, bool is_first_array, bool* isOccupied1, bool* isOccupied2);
bool cuckooArray_query(cuckooArray* arr, elt_t element);
void cuckooArray_delete(cuckooArray* arr);

#endif