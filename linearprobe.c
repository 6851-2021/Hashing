#include "linearprobe.h"
#include "tabulation.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

/*
* Initialize linear probe hash table with a specific table size
* @param tableSize is size of hash table
*/
probeArray* probeArray_initialize(int tableSize) {
    data* start = malloc(tableSize * sizeof(data));
    memset(start, 0, tableSize * sizeof(data));
    table_t* hashTable = hashFuncTable_initialize(30); // 32 had issues
    probeArray* arr = malloc(sizeof(probeArray));
    arr->start = start;
    arr->size = tableSize;
    arr->collisions = 0;
    arr->hashTable = hashTable;
    return arr;
} 

/*
* Insert an element into the linear probe array
* @param arr is the pointer to probe array 
* @param element is the element inserted
*/
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
        arr->collisions++;
    }
}

/*
* Initialize linear probe hash table with a specific table size and elements inserted into it
* @param tableSize is size of hash table
* @param elements is a pointer to array of elements to insert into hash table
* @param numElements is number of elements in param elements
* @return pointer to the probe array
*/
probeArray* probeArray_make(int tableSize, elt_t* elements, int numElements){
    probeArray* arr = probeArray_initialize(tableSize);
    for(int i = 0 ; i < numElements; i++){
        probeArray_insert(arr, elements[i]);
    }
    return arr;
}

/*
* Destructor to remove probeArray struct from heap memory
* @param arr is the pointer to probe array 
*/
void probeArray_delete(probeArray* arr) {
    free(arr->start);
    free(arr->hashTable);
    free(arr);
}

/*
* Check whether an element exists in the linear probe array
* @param arr is the pointer to probe array 
* @param element is the element we query
* @return true if element exists already else false
*/
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