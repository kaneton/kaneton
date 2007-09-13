/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/crypt/kaneton/drivers/vga/vga.c
 *
 * created       matthieu bucchianeri   [tue aug 21 23:15:42 2007]
 * updated       matthieu bucchianeri   [tue sep 11 18:23:27 2007]
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <crt.h>
#include <libkaneton.h>
#include "vga.h"
#include "vga-driver.h"

#include <sys/io.h>

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define PAGED_SIZE(_size_)						\
  ((_size_) % PAGESZ ?							\
   (_size_) + PAGESZ - (_size_) % PAGESZ :				\
   (_size_))

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
 * dump for 640x480x4
 */

static unsigned char g_640x480x4[] =
  {
    0xe3,
    0x03, 0x01, 0x0F, 0x00, 0x06,
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
    0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xEA, 0x8C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
    0xFF,
    0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F,
    0xFF,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x01, 0x00, 0x0F, 0x00, 0x00
  };

/*
 * modes list.
 */

static t_vga_driver_modes modes[] =
  {
    { .width = 320, .height = 200, .bpp = 8, .dump = g_320x200x8,
      .base = 0xa0000 },
    { .width = 640, .height = 480, .bpp = 4, .dump = g_640x480x4,
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

  outb(*regs, VGA_MISC_WRITE);
  regs++;
  for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
    {
      outb(i, VGA_SEQ_INDEX);
      outb(*regs, VGA_SEQ_DATA);
      regs++;
    }
  outb(0x03, VGA_CRTC_INDEX);
  a = inb(VGA_CRTC_DATA);
  outb(a | 0x80, VGA_CRTC_DATA);
  outb(0x11, VGA_CRTC_INDEX);
  a = inb(VGA_CRTC_DATA);
  outb(a & ~0x80, VGA_CRTC_DATA);
  regs[0x03] |= 0x80;
  regs[0x11] &= ~0x80;
  for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
    {
      outb(i, VGA_CRTC_INDEX);
      outb(*regs, VGA_CRTC_DATA);
      regs++;
    }
  for(i = 0; i < VGA_NUM_GC_REGS; i++)
    {
      outb(i, VGA_GC_INDEX);
      outb(*regs, VGA_GC_DATA);
      regs++;
    }
  for(i = 0; i < VGA_NUM_AC_REGS; i++)
    {
      a = inb(VGA_INSTAT_READ);
      outb(i, VGA_AC_INDEX);
      outb(*regs, VGA_AC_WRITE);
      regs++;
    }
  inb(VGA_INSTAT_READ);
  outb(0x20, VGA_AC_INDEX);
}

/*
 * goto specified video mode.
 */

static t_vaddr		vga_goto_mode(i_task	task,
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

      return ((t_vaddr)-1);
    }

  for (i = 0; modes[i].width != -1; i++)
    if (modes[i].width == width && modes[i].height == height &&
	modes[i].bpp == bpp)
      break;

  if (modes[i].width == -1)
    return ((t_vaddr)-1);

  if (task_attribute_asid(task, &asid) != ERROR_NONE)
    return ((t_vaddr)-1);

  if (region_reserve(asid,
		     0x1000,
		     modes[i].base - 0x1000,
		     REGION_OPT_NONE,
		     0,
		     PAGED_SIZE((modes[i].width * modes[i].height *
				 modes[i].bpp) / 8),
		     &reg) != ERROR_NONE)
    return ((t_vaddr)-1);

  printf(" -- vga: entering mode %dx%dx%d\n", width, height, bpp);

  vga.busy = 1;

  write_regs(modes[i].dump);

  return (t_vaddr)reg;
}

/*
 * setup palette.
 */

static void		vga_set_palette(void)
{
/*   outb(VGA_DAC_WRITE_INDEX, 0); */
/*   for (i = 0; i < 256; i++) */
/*     { */
/*       outb(VGA_DAC_DATA, i >> 2); */
/*       outb(VGA_DAC_DATA, i >> 2); */
/*       outb(VGA_DAC_DATA, i >> 2); */
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

  if (message_register(MESSAGE_TYPE_DRIVER_VGA,
		       MESSAGE_SIZE_DRIVER_VGA) != ERROR_NONE)
    return (-1);

  for (port = 0x3C0; port < 0x3DF; port++)
    if (io_grant(port, _crt_get_task_id(), 1) != ERROR_NONE)
      {
	printf(" -- vga: unable to grant access to I/O port 0x%qx\n", port);

	return (-1);
      }

  if (vga_driver_serve())
    return (-1);

  return (0);
}
