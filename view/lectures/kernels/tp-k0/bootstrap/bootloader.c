/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/TP_K0/ex5/bootloader.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [wed jan 24 22:45:41 2007]
 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k0 */

#define CONS_ADDR		0xb8000
#define CONS_COLUMNS		80
#define CONS_LINES		25
#define CONS_BPC		2
#define CONS_DEFAULT_ATTR	0x0f
#define CONS_SIZE		(80 * 25 * 2)

unsigned int		line = 1;
unsigned int		column = 0;
char*			vga = (char *)CONS_ADDR;

void			cons_print_char(char			c)
{
  unsigned short	pos = line * CONS_COLUMNS * CONS_BPC +
    column * CONS_BPC;

  if (c == '\n')
    {
      line++;
      column = 0;

      return ;
    }

  if (pos >= CONS_SIZE)
    {
      line = 0;
      column = 0;
    }

  vga[pos] = c;
  vga[pos + 1] = CONS_DEFAULT_ATTR;
  column++;
}

void			cons_print_string(char*			string)
{
  int			i;

  for (i = 0; string[i]; i++)
    cons_print_char(string[i]);
}

int			bootloader_entry(void)
{
  cons_print_string("[+] kernel is running...\n");

  while (1)
    asm("hlt");
}

/*                                                                 [cut] /k0 */
