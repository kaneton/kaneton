/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/libdata/dump.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [wed jul 26 19:18:02 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>

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
	  dump_buf((u_int8_t*)buf, (u_int) (data + i - j), j);
	  j = 0;
	}
      buf[j++] = data[i];
    }

  if (j > 0)
    dump_buf((u_int8_t*)buf, (u_int) (data + i - j), j);
}

#undef LINEDUMPS
