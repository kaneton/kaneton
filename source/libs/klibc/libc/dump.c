/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * dump.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:50:21 2005   mycure
 * last update   Sun Jun 19 22:23:59 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc/include/klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

#define LINEDUMPS	16

void			dump_buf(u_int8_t*			buf,
				 u_int				address,
				 u_int				size)
{
  u_int			i;

  /*
   * dump address
   */

  printf("0x%08x:     ", address);

  /*
   * dump in hexadecimal format
   */

  for (i = 0; i < LINEDUMPS; i++)
    {
      if (i < size)
	printf("%02x", buf[i]);
      else
	printf("  ");

      if (i % 2)
	printf(" ");
    }

  printf("    ");

  /*
   * dump in string format
   */

  for (i = 0; i < size; i++)
    {
      if (isprint(buf[i]))
	printf("%c", buf[i]);
      else
	printf(".");
    }

  printf("\n");
}

void			dump(char*				data,
			     u_int				size)
{
  char			buf[LINEDUMPS];
  u_int			i;
  u_int			j;

  for (j = 0, i = 0; i < size; i++)
    {
      if (j >= LINEDUMPS)
	{
	  dump_buf(buf, (u_int) (data + i - j), j);
	  j = 0;
	}
      buf[j++] = data[i];
    }

  if (j > 0)
    dump_buf(buf, (u_int) (data + i - j), j);
}

#undef LINEDUMPS
