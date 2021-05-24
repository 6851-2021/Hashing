#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "cuckoo.h"
#include "linearprobe.h"
#include "tabulation.h"
#include "utils.h"

typedef struct {
  double time;
  int collisions;
  int rebuilds;
} result;

void extractChunkTest() {
  assert(extractChunk(127, 0) == 0);
  assert(extractChunk(1023, 7) == 255);
  assert(extractChunk(2030, 7) == 238);
  assert(extractChunk(95, 14) == 0);
}

void testSmallLinearProbing() {
  double epsilon = 1.0;
  int numElements = 20;
  elt_t elements[numElements];
  for (int i = 0; i < numElements; i++) {
    elements[i] = i * 2;
  }

  int tableSize = (int)((1.0 + epsilon) * numElements);

  probeArray* arr = probeArray_make(tableSize, elements, numElements);

  printf("Small Linear Probing:\n");

  for (int i = 0; i < numElements * 2; i++) {
    assert(probeArray_query(arr, i) == (i % 2 == 0));
    // printf("%d",probeArray_query(arr,i));
  }
  probeArray_delete(arr);
  printf("Small Linear Probing passed!\n");
  // printf("\n");
}

void testSmallCuckooHashing() {
  double epsilon = 1.0;
  int numElements = 20;
  elt_t elements[numElements];

  for (int i = 0; i < numElements; i++) {
    elements[i] = i * 2;
  }

  int tableSize = (int)((1.0 + epsilon) * numElements);
  cuckooArray* arr = cuckooArray_make(tableSize, elements, numElements);
  printf("Small Cuckoo Hashing:\n");
  for (int i = 0; i < numElements * 2; i++) {
    assert(cuckooArray_query(arr, i) == (i % 2 == 0));
    printf("%d", cuckooArray_query(arr, i));
  }
  printf("\nSmall Cuckoo Hashing passed!\n");
  cuckooArray_delete(arr);
}

elt_t* getRandomElements(int numElements) {
  elt_t* elements = malloc(numElements * sizeof(elt_t));
  for (int i = 0; i < numElements; i++) {
    elements[i] = randLim(1 << 30) - 1;
  }
  return elements;
}

result* testCuckooHashingPerformance(double epsilon, elt_t* elements,
                                     int numElements) {
  result* res = malloc(sizeof(result));
  int tableSize = (int)((1.0 + epsilon) * numElements);

  clock_t begin = clock();
  cuckooArray* arr = cuckooArray_make(tableSize, elements, numElements);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  for (int i = 0; i < numElements; i++) {
    assert(cuckooArray_query(arr, elements[i]));
    // printf("%d",cuckooArray_query(&arr,i));
  }
  cuckooArray_delete(arr);

  res->time = time_spent;
  res->collisions = arr->collisions;
  res->rebuilds = arr->rebuilds;

  return res;
}

result* testLinearProbingPerformance(double epsilon, elt_t* elements,
                                     int numElements) {
  result* res = malloc(sizeof(result));

  clock_t begin = clock();
  probeArray* arr = probeArray_make((int)((1.0 + epsilon) * numElements),
                                    elements, numElements);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  for (int i = 0; i < numElements; i++) {
    assert(probeArray_query(arr, elements[i]));
  }

  probeArray_delete(arr);

  res->time = time_spent;
  res->collisions = arr->collisions;
  res->rebuilds = 0;

  return res;
}

void testParameters() {
  const int lengthEpsilon = 8;
  const int lengthElements = 1;
  const int numTrials = 10;
  const double epsilonValues[lengthEpsilon] = {0.01, 0.02, 0.05, 0.1,
                                               0.2,  0.5,  1.0,  2.0};
  const int numElementsValues[lengthElements] = {
      1e2};  //, 3e2, 1e3, 3e3, 1e4, 3e4, 1e5};//, 3e5, 1e6, 3e6, 1e7};

  for (int i = 0; i < lengthEpsilon; i++) {
    double epsilon = epsilonValues[i];

    for (int j = 0; j < lengthElements; j++) {
      double linearProbeTime = 0;
      double linearProbeCollisions = 0;
      double cuckooTime = 0;
      double cuckooCollisions = 0;
      double cuckooRebuilds = 0;

      int numElements = numElementsValues[j];

      for (int k = 0; k < numTrials; k++) {
        elt_t* elements = getRandomElements(numElements);

        result* res1 =
            testLinearProbingPerformance(epsilon, elements, numElements);
        result* res2 =
            testCuckooHashingPerformance(epsilon, elements, numElements);
        linearProbeTime += res1->time;
        linearProbeCollisions += res1->collisions;
        cuckooTime += res2->time;
        cuckooCollisions += res2->collisions;
        cuckooRebuilds += res2->rebuilds;
        free(elements);
      }

      linearProbeTime /= numTrials;
      linearProbeCollisions /= numTrials;
      cuckooTime /= numTrials;
      cuckooCollisions /= numTrials;
      cuckooRebuilds /= numTrials;

      printf("--------------------------------------------------\n");
      printf("Linear Probing with %d Elements, eps = %.2f:\n", numElements,
             epsilon);
      printf("Time: %f, Collisions: %.2f\n", linearProbeTime,
             linearProbeCollisions);
      printf("--------------------------------------------------\n");
      printf("Cuckoo Hashing with %d Elements, eps = %.2f:\n", numElements,
             epsilon);
      printf("Time: %f, Collisions: %.2f, Rebuilds: %.2f\n", cuckooTime,
             cuckooCollisions, cuckooRebuilds);
      printf("--------------------------------------------------\n\n");
    }
  }
}

int main() {
  extractChunkTest();

  int numElements = (int)(1e4);

  elt_t* elements = getRandomElements(numElements);

  testSmallLinearProbing();
  testLinearProbingPerformance(1.0, elements, numElements);

  testSmallCuckooHashing();
  testCuckooHashingPerformance(0.01, elements, numElements);

  free(elements);

  testParameters();

  return 0;
}
