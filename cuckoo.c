#include "cuckoo.h"
#include "tabulation.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>



/*
* Initialize cuckoo hash table with a specific table size
* @param tableSize is size of hash table
*/
cuckooArray* cuckooArray_initialize(int tableSize) {
    data* ptr1 = malloc(tableSize * sizeof(data));
    data* ptr2 = malloc(tableSize * sizeof(data));
    memset(ptr1, 0, tableSize * sizeof(data));
    memset(ptr2, 0, tableSize * sizeof(data));
    table_t* hashTable1 = hashFuncTable_initialize(30);
    table_t* hashTable2 = hashFuncTable_initialize(30);
    cuckooArray* arr = malloc(sizeof(cuckooArray));
    arr->start1 = ptr1;
    arr->start2 = ptr2;
    arr->table1 = hashTable1;
    arr->table2 = hashTable2;
    arr->size = tableSize;
    arr->collisions = 0;
    arr->rebuilds = 0;
    return arr;
} 

/*
* Kick element from one of the arrays in the cuckooArray
* @param arr is the pointer to cuckoo array 
* @param elementIndex is the element inserted
* @param isFirstArray true means we kick from first hashtable to second. 
*                   If false, then kick from second hashtable to first.
* @param isOccupied1 is boolean array. Used to reserve spots for elements 
*                   to get kicked to (in the first array), which allows for loop detection,
*                   when we're trying to kick to a reserved spot.
* @param isOccupied2 is boolean array. Same as above, but for the second array
* @return true if insert is successful and false if a loop in the cuckoo kick occur. 
*/
bool cuckooArray_kick(cuckooArray* arr, int elementIndex, bool isFirstArray, bool* isOccupied1, bool* isOccupied2){
    arr->collisions++;

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

/*
* Insert an element into the cuckoo array
* @param arr is the pointer to cuckoo array 
* @param element is the element inserted
* @param isOccupied1 is boolean array initialized to all false. Used to reserve spots for elements 
*                   to get kicked to (in the first array), which allows for loop detection,
*                   when we're trying to kick to a reserved spot.  
* @param isOccupied2 is boolean array initialized to all false. Used to reserve spots for elements 
*                   to get kicked to (in the first array), which allows for loop detection,
*                   when we're trying to kick to a reserved spot.
* @return true if insert is successful and false if a loop in the cuckoo kick occur. 
*/
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


/*
* Insert an array of element into the cuckoo array
* @param arr is the pointer to cuckoo array 
* @param elements is an array of elements inserted
* @param numElements is number of elements in the array elements 
* @return true if insert is successful and false if a loop in a cuckoo kick occur.  
*/
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

bool cuckooArray_query(cuckooArray* arr, elt_t element){
    elt_t possible_element_1 = arr->start1[tabulationHash(arr->table1, element) % arr->size].key;
    elt_t possible_element_2 = arr->start2[tabulationHash(arr->table2, element) % arr->size].key;
    return possible_element_1 == element || possible_element_2 == element;
}

/*
* Destructor to remove cuckooArray struct from heap memory
* @param arr is the pointer to probe array 
*/
void cuckooArray_delete(cuckooArray* arr){
    free(arr->start1);
    free(arr->start2);
    free(arr->table1);   
    free(arr->table2);
    free(arr);
}



/*
* Initialize cuckoo array hash table with a specific table size and elements inserted into it
* @param tableSize is size of hash table
* @param elements is a pointer to array of elements to insert into hash table
* @param numElements is number of elements in param elements
* @return pointer to the probe array
*/
cuckooArray* cuckooArray_make(int tableSize, elt_t* elements, int numElements){
    int numRebuilds = 0;
    while(true){
        cuckooArray* arr = cuckooArray_initialize(tableSize);
        if(cuckooArray_insertAll(arr, elements, numElements)){
            arr->rebuilds = numRebuilds;
            return arr;
        }else{
            numRebuilds++;
            cuckooArray_delete(arr);
        }
    }
}