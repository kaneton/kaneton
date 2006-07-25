#include <stdio.h>

int main(void)
{
  unsigned char buff[512];
  size_t n, i;

  while ((n = fread(buff, 1, 512, stdin)))
    {
      for (i = 0; i < n; i++)
	{
	  printf("\\x%.2x", (buff[i] & 0xFF));
	}
    }
  printf("\n");
  return 0;
}
