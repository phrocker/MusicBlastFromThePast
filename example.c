#include <stdlib.h>

void f() 
{
  int* x = malloc(10 * sizeof(int));
  x[9] = 0;        // problem 1: heap block overrun
  free(x);
}                    // problem 2: memory leak -- x not freed

int main(void) {
  f();
  return 0;
}
