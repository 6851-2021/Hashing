#include "linearprobe.h"
#include "tabulation.h"
#include "stdbool.h"
#include <string.h>

#define CHECK_COLLISIONS


probeArray probeArray_initialize(int tableSize) {
    data* ptr = malloc(tableSize * sizeof(data));
    memset(ptr, 0, tableSize * sizeof(data));
    probeArray arr = {ptr, tableSize, 0};
    return arr;
} 

void probeArray_insert(probeArray arr, long element){
    long hashedElement = tabulationHash(element, CHUNK_SIZE, NUM_CHUNKS);
    int index = hashedElement % arr.size;
    while(true){
        data dataElement = arr.start[index];
        if (!dataElement.exist) {
            data newData = {element, true};
            arr.start[index] = newData;
            return;      
        } else if(dataElement.key == element){
            return;
        }
        index = (index + 1) % arr.size;
#ifdef CHECK_COLLISIONS
        arr.collisions++;
#endif
    }
}

bool probeArray_query(probeArray arr, long element){
    long hashedElement = tabulationHash(element, CHUNK_SIZE, NUM_CHUNKS);
    int index = hashedElement % arr.size;
    while(true){
        data dataElement = arr.start[index];
        if(!dataElement.exist){
            return false;
        }
        else if (dataElement.key == element){
            return true;
        }
        index = (index + 1) % arr.size;
    }
}