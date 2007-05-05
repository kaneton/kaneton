#include <klibc.h>

int main()
{
  char* p = 0xb8000;

  p[0 + 160] = 'T';
  p[2 + 160] = 'e';
  p[4 + 160] = 's';
  p[6 + 160] = 't';

  while (1)
    ;
}
