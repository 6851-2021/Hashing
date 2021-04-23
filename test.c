#include <assert.h>

#include "tabulation.h"
#include "linearprobe.h"
#include <stdio.h>
#include <time.h>

void testSmallLinearProbing(){
  double epsilon = 1.0;
  int numElements = 20;
  probeArray arr = probeArray_initialize((int)((1.0+epsilon)*numElements));
  for(int i = 0; i < numElements; i++){
    probeArray_insert(arr,i*2);
  }
  printf("Small Linear Probing:\n");
  for(int i = 0; i < numElements*2; i++){
    assert(probeArray_query(arr,i) == (i%2==0));
    // printf("%d",probeArray_query(arr,i));
  }
  // printf("\n");
}

void testRandomLinearProbing(double epsilon, int numElements){
  probeArray arr = probeArray_initialize((int)((1.0+epsilon)*numElements));
  int* randomIntegers = malloc(numElements * sizeof(int));
  for(int i = 0; i < numElements; i++){
    int randomInteger = rand();
    probeArray_insert(arr, randomInteger);
    randomIntegers[i] = randomInteger;
  }
  for(int i = 0; i < numElements; i++){
    assert(probeArray_query(arr, randomIntegers[i]));
  }
}

void testLinearProbingPerformance() {
  double epsilon = 1.0;
  int numElements = 1e4;
  int elements[numElements];
  for(int i = 0; i < numElements; i++){
    elements[i] = randLim(1 << 31) - 1;
  }
  probeArray arr = probeArray_initialize((int)((1.0+epsilon)*numElements));

  clock_t begin = clock();
  for(int i = 0; i < numElements; i++){
    probeArray_insert(arr,elements[i]);
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  for(int i = 0; i < numElements; i++){
    assert(probeArray_query(arr,elements[i]));
  }
  printf("Number of collisions: %d\n", arr.collisions);
  
}


int main() {
  assert(extractChunk(127, 4, 0) == 0);
  assert(extractChunk(127, 4, 15) == 15);
  assert(extractChunk(95, 4, 14) == 5);
  //   assert(1 == 0);

  hashFuncTable_initialize(8);
  printf("%lu\n", tabulationHash(200, CHUNK_SIZE, NUM_CHUNKS));

  probeArray arr = probeArray_initialize(32);
  probeArray_insert(arr, 12);
  probeArray_insert(arr, 12);
  printf("%d\n", probeArray_query(arr, 12));
  testSmallLinearProbing();
  testLinearProbingPerformance();


  return 0;
}