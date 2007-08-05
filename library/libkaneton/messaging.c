/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/library/libkaneton/messaging.c
 *
 * created       matthieu bucchianeri   [sun aug  5 23:13:37 2007]
 * updated       matthieu bucchianeri   [sun aug  5 23:50:12 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <core/core.h>
#include <libkaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */


t_error			syscall_message_register(t_type		type,
						 t_vsize	size)
{
  t_error		res;

  asm ("int $56"
       : "=a" (res)
       : "a" (type), "b" (size));

  return res;
}

t_error			syscall_message_send(i_node		destination,
					     t_type		type,
					     t_vaddr		data,
					     t_vsize		size)
{
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;
  t_error		res;

  u.node = destination;

  asm("pushl %%ebp		\n\t"
      "movl %7, %%ebp		\n\t"
      "int $57			\n\t"
      "popl %%ebp"
      : "=a" (res)
      :  "a" (u.reg[0]), "b" (u.reg[1]), "c" (u.reg[2]), "d" (u.reg[3]),
	 "S" (type), "D" (data), "m" (size));

  return res;
}

t_error			syscall_message_transmit(i_node		destination,
						 t_type		type,
						 t_vaddr	data,
						 t_vsize	size)
{
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;
  t_error		res;

  u.node = destination;

  asm("pushl %%ebp		\n\t"
      "movl %7, %%ebp		\n\t"
      "int $58			\n\t"
      "popl %%ebp"
      : "=a" (res)
      :  "a" (u.reg[0]), "b" (u.reg[1]), "c" (u.reg[2]), "d" (u.reg[3]),
	 "S" (type), "D" (data), "m" (size));

  return res;
}

t_error			syscall_message_throw(i_node		destination,
					      t_type		type,
					      t_vaddr		data,
					      t_vsize		size,
					      t_message_request*request)
{
  /* XXX */
}

t_error			syscall_message_receive(t_type		type,
						t_vaddr		data,
						t_vsize*	size,
						i_node*		sender)
{
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;
  t_error		res;
  t_vsize		r_size;

  asm("int $60"
      : "=a" (res), "=b" (r_size), "=c" (u.reg[0]), "=d" (u.reg[1]),
	"=S" (u.reg[2]), "=D" (u.reg[3])
      : "a" (type), "b" (data));

  *size = r_size;
  *sender = u.node;

  return res;
}

t_error			syscall_message_grab(t_type		type,
					     t_vaddr		data,
					     t_message_request*	request)
{
  /* XXX */
}

t_error			syscall_message_poll(t_type		type,
					     t_vaddr		data,
					     t_vsize*		size,
					     i_node*		sender)
{
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;
  t_error		res;
  t_vsize		r_size;

  asm("int $62"
      : "=a" (res), "=b" (r_size), "=c" (u.reg[0]), "=d" (u.reg[1]),
	"=S" (u.reg[2]), "=D" (u.reg[3])
      : "a" (type), "b" (data));

  *size = r_size;
  *sender = u.node;

  return res;
}

t_error			syscall_message_state(t_message_request	request)
{
  /* XXX */
}

t_error			syscall_message_wait(t_message_request	request,
					     t_vsize*		size,
					     i_node*		sender)
{
  /* XXX */
}
