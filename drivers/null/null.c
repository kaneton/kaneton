#include <libc.h>

const char* tutu = "Test";

int main()
{
  char* p = 0xb8000;

  p[0 + 160] = tutu[0];
  p[2 + 160] = tutu[1];
  p[4 + 160] = tutu[2];
  p[6 + 160] = tutu[3];

  while (1)
    ;
}
