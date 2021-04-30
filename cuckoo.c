#include "cuckoo.h"
#include "tabulation.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>


cuckooArray cuckooArray_initialize(int tableSize) {
    data* ptr1 = malloc(tableSize * sizeof(data));
    data* ptr2 = malloc(tableSize * sizeof(data));
    memset(ptr1, 0, tableSize * sizeof(data));
    memset(ptr2, 0, tableSize * sizeof(data));
    table_t* hashTable1 = hashFuncTable_initialize(30);
    table_t* hashTable2 = hashFuncTable_initialize(30);
    cuckooArray arr = {ptr1, ptr2, hashTable1, hashTable2, tableSize, 0};
    return arr;
} 

bool cuckooArray_insertAll(cuckooArray* arr, elt_t* elements, int numElements){
    bool* isOccupied1 = malloc(arr->size * sizeof(bool));
    bool* isOccupied2 = malloc(arr->size * sizeof(bool));
    memset(isOccupied1, 0, arr->size * sizeof(bool));
    memset(isOccupied2, 0, arr->size * sizeof(bool));
    for(int i = 0; i < numElements; i++){
        if(!cuckooArray_insert(arr, elements[i], isOccupied1, isOccupied2)){
            return false;
        }
    }
    return true;
}

bool cuckooArray_insert(cuckooArray* arr, elt_t element, bool* isOccupied1, bool* isOccupied2){
    table_t hashElement1 = tabulationHash(arr->table1, element);
    int index = hashElement1 % arr->size;
    if(arr->start1[index].exist){
        isOccupied1[index] = true;
        if(cuckooArray_kick(arr, index, true, isOccupied1, isOccupied2)) {
            data newData = {element, true};
            arr->start1[index] = newData;
            isOccupied1[index] = false;
            return true;
        } else {
            return false;
        }
    }
    data newData = {element, true};
    arr->start1[index] = newData;
    return true;
}

bool cuckooArray_kick(cuckooArray* arr, int elementIndex, bool isFirstArray, bool* isOccupied1, bool* isOccupied2){
    data* source = isFirstArray ? arr->start1 : arr->start2;
    data* dest = isFirstArray ? arr->start2 : arr->start1;

    bool* destOccupied = isFirstArray ? isOccupied2 : isOccupied1;
    table_t* destinationHashTable = isFirstArray? arr->table2 : arr->table1;
    elt_t element = source[elementIndex].key;
    int destinationIndex = tabulationHash(destinationHashTable,element) % arr->size;

    if(dest[destinationIndex].exist && !destOccupied[destinationIndex]){
        destOccupied[destinationIndex] = true;
        if(!cuckooArray_kick(arr, destinationIndex, !isFirstArray, isOccupied1, isOccupied2)){
            return false;
        }
        data newData = {element, true};
        dest[destinationIndex] = newData;
        destOccupied[destinationIndex] = false;
        return true;
    } else if(!dest[destinationIndex].exist) {
        data newData = {element, true};
        dest[destinationIndex] = newData;
        return true;
    } else {
        return false;
    }
}

bool cuckooArray_query(cuckooArray* arr, elt_t element){
    elt_t possible_element_1 = arr->start1[tabulationHash(arr->table1, element) % arr->size].key;
    elt_t possible_element_2 = arr->start2[tabulationHash(arr->table2, element) % arr->size].key;
    return possible_element_1 == element || possible_element_2 == element;
}


void cuckooArray_delete(cuckooArray* arr){
    free(arr->start1);
    free(arr->start2);
    free(arr->table1);   
    free(arr->table2);
}