#include <stdio.h>
#include "func.h"

int main(int argc, char** argv )
{
  int a = func1( argc );
  int b = func2( argc );
  printf("a from func1=%d\n", a);
  printf("b from func2=%d\n", b);
  return 1;
}
