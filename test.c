#include <assert.h>

#include "tabulation.h"

int main() {
  assert(extractChunk(127, 4, 0) == 0);
  assert(extractChunk(127, 4, 15) == 15);
  assert(extractChunk(95, 4, 14) == 5);
  //   assert(1 == 0);

  initialize(8);
  printf("%d", tabulationHash(200, NUM_CHUNKS));
  return 0;
}