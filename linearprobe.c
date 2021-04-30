#include "linearprobe.h"
#include "tabulation.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

#define CHECK_COLLISIONS


probeArray probeArray_initialize(int tableSize) {
    data* ptr = malloc(tableSize * sizeof(data));
    memset(ptr, 0, tableSize * sizeof(data));
    table_t* hashTable = hashFuncTable_initialize(30);
    probeArray arr = {ptr, tableSize, 0, hashTable};
    return arr;
} 

void probeArray_delete(probeArray* arr) {
    free(arr->start);
    free(arr->hashTable);
}

void probeArray_insert(probeArray* arr, elt_t element){
    table_t hashedElement = tabulationHash(arr->hashTable, element);
    int index = hashedElement % arr->size;
    while(true){
        data dataElement = arr->start[index];
        if (!dataElement.exist) {
            data newData = {element, true};
            arr->start[index] = newData;
            return;
        } else if(dataElement.key == element){
            return;
        }
        index = (index + 1) % arr->size;
#ifdef CHECK_COLLISIONS
        arr->collisions++;
#endif
    }
}

bool probeArray_query(probeArray * arr, elt_t element){
    table_t hashedElement = tabulationHash(arr->hashTable, element);
    int index = hashedElement % arr->size;
    while(true){
        data dataElement = arr->start[index];
        if(!dataElement.exist){
            return false;
        }
        else if (dataElement.key == element){
            return true;
        }
        index = (index + 1) % arr->size;
    }
}