/*
 * licence
 *
 * project       kaneton
 *
 * file          /home/buckman/crypt/kaneton/kaneton/core/debug/cons-vga.c
 *
 * created       matthieu bucchianeri   [fri dec  2 20:14:33 2005]
 * updated       matthieu bucchianeri   [tue feb 13 16:37:06 2007]
 */

/*
 * http://my.execpc.com/~geezer/osd/graphics/modes.c
 *
 * an example from which this file was written.
 */

/*
 * mode selection (MODE2 is not working)
 */
#define MODE1		// 1: 640x480x2, 2: 640x480x16, 3: 320x200x256

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <platform/platform.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the console variable.
 */

t_cons			cons;
t_fbcons		fbcons;

/*
 * modes
 */

IBMPC_VGA_DUMP_640_480_2(g_640x480x2);
IBMPC_VGA_DUMP_640_480_16(g_640x480x16);
IBMPC_VGA_DUMP_320_200_256(g_320x200x256);

/*
 * font
 */

IBMPC_VGA_FONT_8_8(g_8x8_font);

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * write vga register dump to the card.
 *
 */

void		write_regs(unsigned char *regs)
{
  unsigned int	i, a;

  OUTB(VGA_MISC_WRITE, *regs);
  regs++;
  for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
    {
      OUTB(VGA_SEQ_INDEX, i);
      OUTB(VGA_SEQ_DATA, *regs);
      regs++;
    }
  OUTB(VGA_CRTC_INDEX, 0x03);
  INB(VGA_CRTC_DATA, a);
  OUTB(VGA_CRTC_DATA, a | 0x80);
  OUTB(VGA_CRTC_INDEX, 0x11);
  INB(VGA_CRTC_DATA, a);
  OUTB(VGA_CRTC_DATA, a & ~0x80);
  regs[0x03] |= 0x80;
  regs[0x11] &= ~0x80;
  for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
    {
      OUTB(VGA_CRTC_INDEX, i);
      OUTB(VGA_CRTC_DATA, *regs);
      regs++;
    }
  for(i = 0; i < VGA_NUM_GC_REGS; i++)
    {
      OUTB(VGA_GC_INDEX, i);
      OUTB(VGA_GC_DATA, *regs);
      regs++;
    }
  for(i = 0; i < VGA_NUM_AC_REGS; i++)
    {
      INB(VGA_INSTAT_READ, a);
      OUTB(VGA_AC_INDEX, i);
      OUTB(VGA_AC_WRITE, *regs);
      regs++;
    }
  INB(VGA_INSTAT_READ, a);
  OUTB(VGA_AC_INDEX, 0x20);
}

/*
 * get base vga segment.
 *
 */

static char*	get_fb_seg(void)
{
  unsigned int	seg;

  OUTB(VGA_GC_INDEX, 6);
  INB(VGA_GC_DATA, seg);
  seg >>= 2;
  seg &= 3;
  switch(seg)
    {
      case 0:
      case 1:
	seg = 0xA0000;
		break;
      case 2:
	seg = 0xB0000;
	break;
      case 3:
	seg = 0xB8000;
	break;
    }
  return (char *)seg;
}

/*
 * this function just clears the console.
 */

void			cons_clear(void)
{
  t_uint16		i;

  for (i = 0; i < fbcons.size; i++)
    {
      fbcons.vga[i] = 0x0;
    }

  cons.line = 0;
  cons.column = 0;
}

/*
 * this function scrolls the screen.
 */

void			cons_scroll(t_uint16			lines)
{
  unsigned int		ln;

  ln = (fbcons.width * 8) / (8 / fbcons.bpp);
  memcpy(fbcons.vga, fbcons.vga + ln, fbcons.size - ln);
  memset(fbcons.vga + fbcons.size - ln, 0, ln);

  cons.column = 0;
  cons.line -= lines;
}

/*
 * this function is called by the printf function to change the
 * current console attributes with the sequence %#.
 */

void			cons_attr(t_uint8			attr)
{
  cons.attr = attr;
}

/*
 * used to decompose a byte
 */
static int bit_on(char c, int n)
{
  int	mask;

  mask = 1 << (7 - n);
  return c & mask;
}

/*
 * this function is called by the printf function to print a
 * character.
 */

int			cons_print_char(char			c)
{
  int			pos;
  int			i, j;
  int			fg;
  int			bg;
  char			ch, p;

  if (c == '\n')
    {
      cons.line++;
      cons.column = 0;

      return 1;
    }

  if (c == '\r')
    {
      cons.column = 0;

      return 1;
    }

  if (cons.column == fbcons.columns)
    {
      ++cons.line;
      cons.column = 0;
    }

  if (cons.line == fbcons.lines)
    {
      cons_scroll(1);
    }

  if (fbcons.bpp == 8)
    {
      fg = cons.attr & 0xF;
      bg = (cons.attr >> 4) & 0xF;

      for (i = 0; i < 8; ++i)
	for (j = 0; j < 8; ++j)
	  {
	    ch = g_8x8_font[c * 8 + i];
	    p = bit_on(ch, j) ? fg : bg;

	    pos = ((cons.line * 8 + i) * fbcons.width) +
	      (cons.column * 8) + j;
	    fbcons.vga[pos] = p;
	  }
    }
  else if (fbcons.bpp == 4)
    {
      fg = cons.attr & 0xF;
      bg = (cons.attr >> 4) & 0xF;

      for (i = 0; i < 8; ++i)
	for (j = 0; j < 4; ++j)
	  {
	    ch = g_8x8_font[c * 8 + i];
	    p = bit_on(ch, j) ? fg : bg;
	    p |= ((bit_on(ch, j + 1) ? fg : bg) << 4);

	    pos = ((cons.line * 8 + i) * fbcons.width) / 2 +
	      (cons.column * 8) / 2 + j;
	    fbcons.vga[pos] = p;
	  }
    }
  else if (fbcons.bpp == 1)
    {
      for (i = 0; i < 8; ++i)
	{
	  fbcons.vga[(cons.line * 8 + i) * fbcons.width / 8 +
		     cons.column] = g_8x8_font[c * 8 + i];
	}
    }
  cons.column++;

  return (1);
}

/*
 * this function just prints a string.
 */

void			cons_print_string(char*			string)
{
  t_uint32		i;

  for (i = 0; string[i]; i++)
    cons_print_char(string[i]);
}

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			cons_msg(char				indicator,
				 char*				fmt,
				 ...)
{
  t_uint8		attr = cons.attr;
  va_list		args;

  cons.attr = CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  cons_print_char('[');

  switch (indicator)
    {
    case '+':
      cons.attr = CONS_FRONT(CONS_GREEN) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    case '#':
      cons.attr = CONS_FRONT(CONS_MAGENTA) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    case '!':
      cons.attr = CONS_FRONT(CONS_RED) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    }

  cons_print_char(indicator);

  cons.attr = CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  cons_print_string("] ");

  cons.attr = attr;

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);
}

/*
 * this function just initializes the bootloader console.
 *
 * this function also initializes the printf function to work with the
 * console.
 */

t_error			cons_init(void)
{
  char			buff[10 * CONS_COLUMNS * CONS_BPC];
  int			prevx, prevy, i, j;
  int			pos;
  int			attr;

  cons.attr = CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  cons.vga = (char*)CONS_ADDR;
  memcpy(buff, cons.vga, 10 * CONS_COLUMNS * CONS_BPC);

#ifdef MODE1
  write_regs(g_640x480x2);
  fbcons.width = 640;
  fbcons.columns = 80;
  fbcons.height = 480;
  fbcons.lines = 60;
  fbcons.bpp = 1;
#endif
#ifdef MODE2
  write_regs(g_640x480x16);
  fbcons.width = 640;
  fbcons.columns = 80;
  fbcons.height = 480;
  fbcons.lines = 60;
  fbcons.bpp = 4;
#endif
#ifdef MODE3
  write_regs(g_320x200x256);
  fbcons.width = 320;
  fbcons.columns = 40;
  fbcons.height = 200;
  fbcons.lines = 25;
  fbcons.bpp = 8;
#endif
  fbcons.size = (fbcons.width * fbcons.height) / (8 / fbcons.bpp);
  fbcons.vga = get_fb_seg();
  prevx = cons.column;
  prevy = cons.line;
  cons_clear();
  attr = cons.attr;
  for (i = 0; i < prevy; ++i)
    for (j = 0; j < CONS_COLUMNS; ++j)
      {
	pos = i * CONS_COLUMNS * CONS_BPC + j * CONS_BPC;
	cons_attr(buff[pos + 1]);
	cons_print_char(buff[pos]);
      }
  cons.attr = attr;
  printf_init(cons_print_char, cons_attr);

  return (ERROR_NONE);
}

/*
 * this function just reinitializes the bootloader console.
 *
 * there is nothing special to do.
 */

t_error			cons_clean(void)
{
  return (ERROR_NONE);
}

