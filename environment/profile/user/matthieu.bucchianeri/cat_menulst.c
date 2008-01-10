#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static char buf[300 * 1024];

#define MENU_START	0x19d3d
#define MENU_END	0x1a210

int main(int argc, char **argv)
{
  FILE *f, *chiche;
  int r;
  int spc = MENU_END - MENU_START;
  char zero = 0;

  if ((f = fopen("pxegrub.template", "r")) == NULL ||
      (chiche = fopen(argv[1], "r")) == NULL)
    return 1;

  fread(buf, 1, MENU_START, f);

  fwrite(buf, 1, MENU_START, stdout);

  fseek(f, spc, SEEK_CUR);

  r = fread(buf, 1, spc, chiche);

  fwrite(buf, 1, r, stdout);
  spc -= r;

  for (r = 0; r < spc; r++)
    fwrite(&zero, 1, 1, stdout);

  r = fread(buf, 1, 0x100000, f);

  fwrite(buf, 1, r, stdout);

  fclose(f);

  return 0;
}
