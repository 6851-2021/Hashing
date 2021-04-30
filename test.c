#include <assert.h>

#include "cuckoo.h"
#include "tabulation.h"
#include "linearprobe.h"
#include <stdio.h>
#include <time.h>
#include "util.h"

void testSmallLinearProbing(){
  double epsilon = 1.0;
  int numElements = 20;
  probeArray arr = probeArray_initialize((int)((1.0+epsilon)*numElements));

  for(int i = 0; i < numElements; i++){
    probeArray_insert(&arr,i*2);
  }

  printf("Small Linear Probing:\n");

  for(int i = 0; i < numElements * 2; i++){
    assert(probeArray_query(&arr,i) == (i%2==0));
    // printf("%d",probeArray_query(arr,i));
  }
  probeArray_delete(&arr);
  printf("Small Linear Probing passed!\n");
  // printf("\n");
}

void testSmallCuckooHashing(){
  double epsilon = 1.0;
  int numElements = 20;
  elt_t* elements = malloc(numElements*sizeof(elt_t));

  for(int i = 0; i < numElements; i++){
    elements[i] = i*2;
  }
  while(true){
    cuckooArray arr = cuckooArray_initialize((int)((1.0+epsilon)*numElements));
    if(cuckooArray_insertAll(&arr, elements, numElements)){
      printf("Small Cuckoo Hashing:\n");

      for(int i = 0; i < numElements * 2; i++){
        assert(cuckooArray_query(&arr,i) == (i%2==0));
        printf("%d",cuckooArray_query(&arr,i));
      }

      printf("\nSmall Cuckoo Hashing passed!\n");
      // printf("\n");
      break;
    }
    cuckooArray_delete(&arr);
  }
}

void testRandomCuckooHashing(double epsilon, int numElements){
  elt_t* elements = malloc(numElements*sizeof(elt_t));

  printf("Random Cuckoo Hashing:\n");

  for(int i = 0; i < numElements; i++){
    elements[i] = randLim(1 << 30) - 1;
  }
  
  printf("Random elements initialized...\n");
  int numRebuilds = 0;

  while(true){
    cuckooArray arr = cuckooArray_initialize((int)((1.0+epsilon)*numElements));
    if(cuckooArray_insertAll(&arr, elements, numElements)) {
      
      for(int i = 0; i < numElements; i++){
        assert(cuckooArray_query(&arr, elements[i]));
        //printf("%d",cuckooArray_query(&arr,i));
      }

      printf("\nRandom Cuckoo Hashing passed! Rebuilds: %d\n", numRebuilds);
      // printf("\n");
      break;
    }
    if(numRebuilds % 10 == 9) {
      printf("Rebuilds so far: %d\n", numRebuilds+1);
    }
    cuckooArray_delete(&arr);
    numRebuilds++;
  }

}

// 400M bytes -> 400K KB -> 400 MB 

void testLinearProbingPerformance(double epsilon, int numElements) {
  int * elements = malloc(numElements * sizeof(int));
  printf("Before loop\n");
  for(int i = 0; i < numElements; i++){
    elements[i] = randLim(1 << 30) - 1;
  }
  printf("Random elements initialized... \n");
  probeArray arr = probeArray_initialize((int)((1.0 + epsilon) * numElements));

  clock_t begin = clock();
  for(int i = 0; i < numElements; i++){
    probeArray_insert(&arr, elements[i]);
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  for(int i = 0; i < numElements; i++){
    assert(probeArray_query(&arr, elements[i]));
  }
  printf("Number of collisions: %d\n", arr.collisions);
  free(elements);
  probeArray_delete(&arr);
}


int main() {
  assert(extractChunk(127, 0) == 0);
  assert(extractChunk(1023, 7) == 255);
  assert(extractChunk(2030, 7) == 238);
  assert(extractChunk(95, 14) == 0);

  probeArray arr = probeArray_initialize(32);
  probeArray_insert(&arr, 12);
  probeArray_insert(&arr, 12);
  printf("%d\n", probeArray_query(&arr, 12));
  testSmallLinearProbing();
  // testLinearProbingPerformance(1.0, 1e6);

  testSmallCuckooHashing();
  testRandomCuckooHashing(0.01, 1e7);
  return 0;
}