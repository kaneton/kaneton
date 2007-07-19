#include <asm/io.h>

int main(int argc, char **argv)
{
  ioperm(0x378, 1, 1);
  outb(1, 0x378);
  usleep(200000);
  outb(0, 0x378);

  return 0;
}
