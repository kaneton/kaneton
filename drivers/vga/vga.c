/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/drivers/vga/vga.c
 *
 * created       matthieu bucchianeri   [tue aug 21 23:15:42 2007]
 * updated       matthieu bucchianeri   [thu aug 23 17:14:58 2007]
 */

#include <libc.h>
#include <crt.h>
#include <libkaneton.h>
#include "vga.h"
#include "vga-driver.h"

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define PAGED_SIZE(_size_)						\
  ((_size_) % PAGESZ ?							\
   (_size_) + PAGESZ - (_size_) % PAGESZ :				\
   (_size_))

/* XXX elsewhere */

#define		OUTB(_port_, _data_)					\
  asm volatile("outb %%al, %%dx\n"					\
	       :							\
	       : "d" (_port_), "a" (_data_))

#define		INB(_port_, _data_)					\
  asm volatile("inb %%dx, %%al\n"					\
	       : "=a" (_data_)						\
	       : "d" (_port_))

/*
 * ---------- globals  --------------------------------------------------------
 */

/*
 * dump for 320x200x8
 */

static unsigned char g_320x200x8[] =
  {
    0x63,
    0x03, 0x01, 0x0F, 0x00, 0x0E,
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
    0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    0xFF,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00, 0x00
  };

/*
 * modes list.
 */

static t_vga_driver_modes modes[] =
  {
    { .width = 320, .height = 200, .bpp = 8, .dump = g_320x200x8,
      .base = 0xa0000 },
    { .width = -1, .height = -1, .bpp = -1, .dump = NULL, .base = 0x0 }
  };

/*
 * state variable.
 */

static t_vga	vga;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * write a register dump into the video controllers.
 */

static void		write_regs(unsigned char *regs)
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
 * goto specified video mode.
 */

static vaddr_t		vga_goto_mode(i_task	task,
				      int32_t	width,
				      int32_t	height,
				      int32_t	bpp)
{
  int			i;
  i_as			asid;
  i_region		reg;

  if (vga.busy)
    {
      printf(" -- vga: cannot switch mode: device alreay in use.\n");

      return ((vaddr_t)-1);
    }

  for (i = 0; modes[i].width != -1; i++)
    if (modes[i].width == width && modes[i].height == height &&
	modes[i].bpp == bpp)
      break;

  if (modes[i].width == -1)
    return ((vaddr_t)-1);

  if (task_attribute_asid(task, &asid) != ERROR_NONE)
    return ((vaddr_t)-1);

  if (region_reserve(asid,
		     0x1000,
		     modes[i].base - 0x1000,
		     REGION_OPT_NONE,
		     0,
		     PAGED_SIZE(modes[i].width * modes[i].height *
				modes[i].bpp / 8),
		     &reg) != ERROR_NONE)
    return ((vaddr_t)-1);

  printf(" -- vga: entering mode %dx%dx%d\n", width, height, bpp);

  vga.busy = 1;

  write_regs(modes[i].dump);

  return (vaddr_t)reg;
}

/*
 * setup palette.
 */

static void		vga_set_palette(void)
{
/*   OUTB(VGA_DAC_WRITE_INDEX, 0); */
/*   for (i = 0; i < 256; i++) */
/*     { */
/*       OUTB(VGA_DAC_DATA, i >> 2); */
/*       OUTB(VGA_DAC_DATA, i >> 2); */
/*       OUTB(VGA_DAC_DATA, i >> 2); */
/*     } */
}

/*
 * serve driver requests.
 */

int			vga_driver_serve(void)
{
  t_driver_vga_message*	message;
  i_node	       	sender;
  t_vsize	       	size;

  if ((message = malloc(sizeof (*message))) == NULL)
    {
      printf(" -- vga: memory exhausted\n");
      return (-1);
    }

  while (1)
    {
      if (message_receive(MESSAGE_TYPE_DRIVER_VGA,
			  (t_vaddr)message,
			  &size,
			  &sender) == ERROR_NONE)
	{
	  if (message->u.request.operation == VGA_DRIVER_FRAMEBUFFER)
	    {
	      /* XXX critical section begins */
	      printf("youhou\n");

	      message->u.reply.u.framebuffer.base =
		vga_goto_mode(sender.task,
			      message->u.request.u.framebuffer.width,
			      message->u.request.u.framebuffer.height,
			      message->u.request.u.framebuffer.bpp);

	      /* XXX critical section ends */

	      if (message_send(sender,
			       MESSAGE_TYPE_DRIVER_VGA,
			       (t_vaddr)message,
			       sizeof (*message)) != ERROR_NONE)
		{
		  printf(" -- vga: error in request\n");
		}
	    }
	}
    }

  return (0);
}

/*
 * driver main.
 */

int		main()
{
  i_port	port;

  vga.busy = 0;

  printf(" -- vga: VGA Framebuffer driver started.\n");

  for (port = 0x3C0; port < 0x3DF; port++)
    if (io_grant(port, _crt_get_task_id(), 1) != ERROR_NONE)
      {
	printf(" -- vga: unable to grant access to I/O port 0x%qx\n", port);

	return (-1);
      }

  if (message_register(MESSAGE_TYPE_DRIVER_VGA,
		       MESSAGE_SIZE_DRIVER_VGA) != ERROR_NONE)
    return (-1);

  if (vga_driver_serve())
    return (-1);

  return (0);
}
