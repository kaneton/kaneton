/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/k4_tests/kaneton/check/kaneton/core/message/common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [sun jun 10 19:09:33 2007]
 */

#include <libc.h>
#include <kaneton.h>
#include "common.h"

void	check_message_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

static unsigned char	bcd2bin(unsigned char c)
{
  unsigned char res = 0;

  unsigned char c1 = (c & 0xF0) >> 4;
  res += 10 * c1;
  c1 = c & 0x0F;
  res += c1;
  return res;
}

/*
 * get seconds from RTC
 */
#if 0
t_uint32 check_cmos_sec(void)
{
  int i;

  OUTB(0x70, 0);
  INB(0x71, i);
  return bcd2bin(i);
}
#endif
/*
 * starts ALL tests related to time manager
 */

void	check_message_tests(void)
{
  check_message_common();
  /* XXX continue tests */
}
#if 0
int		check_task_create(t_class		class,
				  i_task		*task)
{
  i_as		as;

  if (task_reserve(class,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   task) != ERROR_NONE)
    {
      printf("error in task_reserve\n");
      return -1;
    }

  if (as_reserve(*task, &as) != ERROR_NONE)
    {
      printf("error in as_reserve\n");
      return -1;
    }

  return 0;
}

int		check_thread_create(i_task		task,
				    t_prior		prior,
				    t_vaddr		entry,
				    i_thread		*thread)
{
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;

  if (thread_reserve(task, prior, thread) != ERROR_NONE)
    {
      printf("error in thread_reserve\n");
      return -1;
    }

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(*thread, stack) != ERROR_NONE)
    {
      printf("error in thread_stack\n");
      return -1;
    }

  if (thread_get(*thread, &o) != ERROR_NONE)
    {
      printf("error in thread_get\n");
      return -1;
    }

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)entry;

  if (thread_load(*thread, ctx) != ERROR_NONE)
    {
      printf("error in thread_get\n");
      return -1;
    }

  return 0;
}
#endif

t_error			syscall_message_register(t_type		type,
						 t_vsize	size)
{
  t_error		res;

#ifdef MESSAGE_CHECKSUM_ENABLE
  size += sizeof (t_uint32);
#endif

  asm ("int $56"
       : "=a" (res)
       : "a" (type), "b" (size));

#ifdef MESSAGE_CHECKSUM_ENABLE
  t_vsize		sz;

  if (message_size(type, &sz) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (size != sz + 4)
    printf("message: bad message box size\n");
#endif

  return res;
}

t_error			syscall_message_size(t_type		type,
					     t_vsize*		size)
{
  t_error		res;
  t_vsize		r_size;

  asm ("int $65"
       : "=a" (res), "=b" (r_size)
       : "a" (type));

#ifdef MESSAGE_CHECKSUM_ENABLE
  r_size -= sizeof (t_uint32);
#endif

  *size = r_size;

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

#ifdef MESSAGE_CHECKSUM_ENABLE
  void*			p;
  t_uint32*		chk;
  t_uint8		inf[sizeof (o_syscall) + sizeof (t_uint32)];

  if (destination.task == 0 && type == MESSAGE_TYPE_INTERFACE)
    p = (void*)inf;
  else
    if ((p = malloc(size + sizeof (t_uint32))) == NULL)
      return (ERROR_UNKNOWN);

  memcpy(p, (void*)data, size);

  chk = (t_uint32*)((t_uint8*)p + size);
  *chk = sum2((void*)data, size);

  data = (t_vaddr)p;
#endif

  asm("pushl %%ebp		\n\t"
      "movl %7, %%ebp		\n\t"
      "int $57			\n\t"
      "popl %%ebp"
      : "=a" (res)
      :  "a" (u.reg[0]), "b" (u.reg[1]), "c" (u.reg[2]), "d" (u.reg[3]),
	 "S" (type), "D" (data), "m" (size));

#ifdef MESSAGE_CHECKSUM_ENABLE
  if (!(destination.task == 0 && type == MESSAGE_TYPE_INTERFACE))
    free(p);
#endif

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

#ifdef MESSAGE_CHECKSUM_ENABLE
  void*			p;
  t_uint32*		chk;

  if ((p = malloc(size + sizeof (t_uint32))) == NULL)
    return (ERROR_UNKNOWN);

  memcpy(p, (void*)data, size);

  chk = (t_uint32*)((t_uint8*)p + size);
  *chk = sum2((void*)data, size);

  data = (t_vaddr)p;
#endif

  asm("pushl %%ebp		\n\t"
      "movl %7, %%ebp		\n\t"
      "int $58			\n\t"
      "popl %%ebp"
      : "=a" (res)
      :  "a" (u.reg[0]), "b" (u.reg[1]), "c" (u.reg[2]), "d" (u.reg[3]),
	 "S" (type), "D" (data), "m" (size));

#ifdef MESSAGE_CHECKSUM_ENABLE
  free(p);
#endif

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

#ifdef MESSAGE_CHECKSUM_ENABLE
  void*			p;
  t_vaddr		real_data = data;
  t_vsize		sz;
  t_uint8		inf[sizeof (o_syscall) + sizeof (t_uint32)];

  if (type == MESSAGE_TYPE_INTERFACE)
    p = (void*)inf;
  else
    {
      if (message_size(type, &sz) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if ((p = malloc(sz + sizeof (t_uint32))) == NULL)
	return (ERROR_UNKNOWN);
    }

  data = (t_vaddr)p;
#endif

  asm("int $60"
      : "=a" (res), "=b" (r_size), "=c" (u.reg[0]), "=d" (u.reg[1]),
	"=S" (u.reg[2]), "=D" (u.reg[3])
      : "a" (type), "b" (data));

#ifdef MESSAGE_CHECKSUM_ENABLE
  t_uint32*		chk;

  r_size -= sizeof (t_uint32);

  chk = (t_uint32*)((t_uint8*)data + r_size);
  if (*chk != sum2((void*)data, r_size))
    printf("message: bad checksum\n");

  memcpy((void*)real_data, (void*)data, r_size);

  if (!(type == MESSAGE_TYPE_INTERFACE))
    free(p);
#endif

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

#ifdef MESSAGE_CHECKSUM_ENABLE
  void*			p;
  t_vaddr		real_data = data;
  t_vsize		sz;

  if (message_size(type, &sz) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if ((p = malloc(sz + sizeof (t_uint32))) == NULL)
    return (ERROR_UNKNOWN);

  data = (t_vaddr)p;
#endif

  asm("int $62"
      : "=a" (res), "=b" (r_size), "=c" (u.reg[0]), "=d" (u.reg[1]),
	"=S" (u.reg[2]), "=D" (u.reg[3])
      : "a" (type), "b" (data));

  *size = r_size;
  *sender = u.node;

#ifdef MESSAGE_CHECKSUM_ENABLE
  t_uint32*		chk;

  r_size -= sizeof (t_uint32);

  chk = (t_uint32*)((t_uint8*)data + r_size);
  if (*chk != sum2((void*)data, r_size))
    printf("message: bad checksum\n");

  memcpy((void*)real_data, (void*)data, r_size);

  free(p);
#endif

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

/*
 * special send with unlocking.
 */

t_error			syscall_message_send_unlock(i_node		dest,
						    t_type		type,
						    t_vaddr		data,
						    t_vsize		size,
						    t_uint8*		lock)
{
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;
  t_error		res;

  u.node = dest;

#ifdef MESSAGE_CHECKSUM_ENABLE
  void*			p;
  t_uint32*		chk;

  if ((p = malloc(size + sizeof (t_uint32))) == NULL)
    return (ERROR_UNKNOWN);

  memcpy(p, (void*)data, size);

  chk = (t_uint32*)((t_uint8*)p + size);
  *chk = sum2((void*)data, size);

  data = (t_vaddr)p;
#endif

  *lock = 0; /* XXX should be done in assembly below... but not enough regs! */

  asm("pushl %%ebp		\n\t"
      "movl %7, %%ebp		\n\t"
      "int $57			\n\t"
      "popl %%ebp"
      : "=a" (res)
      :  "a" (u.reg[0]), "b" (u.reg[1]), "c" (u.reg[2]), "d" (u.reg[3]),
	 "S" (type), "D" (data), "m" (size), "m" (lock));

#ifdef MESSAGE_CHECKSUM_ENABLE
  free(p);
#endif

  return res;
}
