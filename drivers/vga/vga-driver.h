/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/crypt/kaneton/drivers/vga/vga-driver.h
 *
 * created       matthieu bucchianeri   [thu aug 23 16:09:27 2007]
 * updated       matthieu bucchianeri   [tue sep 11 18:24:26 2007]
 */

#ifndef DRIVERS_VGA_DRIVER_H
#define DRIVERS_VGA_DRIVER_H

/*
 * ---------- information -----------------------------------------------------
 *
 * vga driver interface.
 *
 * the driver replies to the following requests:
 *
 * - FRAMEBUFFER(t_driver_vga_metric width, height, bpp)
 *   switch to specified graphic mode and return a framebuffer base.
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <stdint.h>
#include <crt.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * driver name.
 */

#define DRIVER_VGA_NAME			"vga"

/*
 * operations.
 */

#define	VGA_DRIVER_FRAMEBUFFER		1

/*
 * message types.
 */

#define MESSAGE_TYPE_DRIVER_VGA		(MESSAGE_TYPE_CRT + 2)
#define MESSAGE_SIZE_DRIVER_VGA		(sizeof (t_driver_vga_message))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * basic types.
 */

typedef int32_t		t_driver_vga_metric;

/*
 * message type.
 */

typedef struct
{
  union
  {
    struct
    {
      t_id			operation;
      union
      {
	struct
	{
	  t_driver_vga_metric		width;
	  t_driver_vga_metric		height;
	  t_driver_vga_metric		bpp;
	}				framebuffer;
      }			u;
    }			request;
    struct
    {
      union
      {
	struct
	{
	  t_vaddr			base;
	}				framebuffer;
      }			u;
    }			reply;
  } u;
}			t_driver_vga_message;

/*
 * ---------- interface -------------------------------------------------------
 */

#ifdef VGA_SPAWN_INTERFACE

#define VGA_ATTRIBUTE_DEFAULT		static inline

#ifndef VGA_ATTRIBUTE_INTERFACE
# define VGA_ATTRIBUTE_INTERFACE	VGA_ATTRIBUTE_DEFAULT
#endif

VGA_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
vga_init(void)
{
  return message_register(MESSAGE_TYPE_DRIVER_VGA, MESSAGE_SIZE_DRIVER_VGA);
}

VGA_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
vga_framebuffer(i_task vga, t_driver_vga_metric width,
		t_driver_vga_metric height, t_driver_vga_metric bpp,
		t_vaddr* base)
{
  t_driver_vga_message*	message;
  i_node		vga_driver;
  t_vsize		size;

  vga_driver.machine = 0;
  vga_driver.task = vga;

  if ((message = malloc(sizeof (*message))) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = VGA_DRIVER_FRAMEBUFFER;
  message->u.request.u.framebuffer.width = width;
  message->u.request.u.framebuffer.height = height;
  message->u.request.u.framebuffer.bpp = bpp;

  if (message_send(vga_driver,
		   MESSAGE_TYPE_DRIVER_VGA, (t_vaddr)message,
		   sizeof (*message)) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  if (message_receive(MESSAGE_TYPE_DRIVER_VGA, (t_vaddr)message,
		      &size, &vga_driver) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  if (base != NULL)
    *base = message->u.reply.u.framebuffer.base;

  free(message);

  return (ERROR_NONE);
}

#endif

#endif
