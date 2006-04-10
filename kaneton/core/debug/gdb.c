/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/debug/gdb.c
 *
 * created       matthieu bucchianeri   [mon apr 10 12:47:20 2006]
 * updated       matthieu bucchianeri   [mon apr 10 23:21:07 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements a gdb stub for kaneton.
 *
 * this stub  is not very complete  but gives the  minimum required to
 * debug the kernel remotely.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * gdb context dump (done by the event handler).
 */

typedef struct
{
  t_uint32	fs;
  t_uint32	es;
  t_uint32	ds;
  t_uint32	esi;
  t_uint32	edi;
  t_uint32	edx;
  t_uint32	ecx;
  t_uint32	ebx;
  t_uint32	eax;
  t_uint8	reserved3[0x14];
  t_uint32	reserved2;
  t_uint32	ebp;
  t_uint32	eip;
  t_uint32	cs;
  t_uint32	eflags;
}		__attribute__ ((packed)) t_gdb_context;

/*
 * simple function pointer.
 */

typedef int (*t_pfngdb)();

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the serial port to communicate with.
 */

static const t_uint32		com_port = SERIAL_COM1;

/*
 * step-by-step mode enabled.
 */

static volatile t_uint32	step = 0;

/*
 * the breakpoints addresses set.
 */

static t_setid			br = ID_UNUSED;

/*
 * the commands list.
 */

static const struct {
  t_uint8	cmd;
  const char*	desc;
  t_pfngdb	fun;
} commands[] =
  {
    { 'g', "read registers", gdb_read_reg },
    { 'G', "write registers", gdb_write_reg },
    { 'm', "read memory", gdb_read_mem },
    { 'M', "write memory", gdb_write_mem },
    { 's', "step", gdb_step },
    { 'c', "continue", gdb_continue },
    { 'z', "unset breakpoint", gdb_unset_break },
    { 'Z', "set breakpoint", gdb_set_break },
    { '?', "status ?", gdb_status },
    { 0, 0, 0 }
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this is the debug handler, called after each machine instruction.
 *
 * steps:
 *
 * 1) compute the context frame address.
 * 2) stops the execution if needed.
 */

void			gdb_handler(t_uint32 needless)
{
  t_gdb_context*	ctx;
  t_uint8*		ptr;
  t_id			id;

  /*
   * 1)
   */

  asm volatile("movl (%%ebp), %%eax\n\t"
	       "movl (%%eax), %%ebx\n\t"
	       "movl %%ebx, %0"
	       : "=g" (ptr)
	       :
	       : "%eax", "%ebx");

  ptr -= 60;
  ctx = (t_gdb_context*)ptr;

  /*
   * 2)
   */

  if (step || set_get(br, ctx->eip, (void**)&id) == ERROR_NONE)
    {
      gdb_status(NULL);
      gdb_command(ctx);
    }
}

/*
 * this function compute the checksum of a gdb packet.
 *
 * steps:
 *
 * 1) compute the sum.
 * 2) build the string.
 */

static void	gdb_checksum(t_uint8*		packet,
			     t_uint8*		chksum)
{
  t_uint32	sum = 0;

  /*
   * 1)
   */

  for (; *packet; packet++)
    sum += *packet;
  sum = sum % 256;

  /*
   * 2)
   */

  chksum[0] = (sum >> 4) & 0xF;
  if (chksum[0] >= 10)
    chksum[0] = 'a' + (chksum[0] - 10);
  else
    chksum[0] += '0';
  chksum[1] = (sum & 0xF);
  if (chksum[1] >= 10)
    chksum[1] = 'a' + (chksum[1] - 10);
  else
    chksum[1] += '0';
}

/*
 * this function sends a gdb formatted packet.
 *
 * steps:
 * 1) compute the checksum.
 * 2) send the packet.
 * 3) read acknowledgment.
 */

static void	gdb_send(t_uint8*		packet)
{
  t_uint8	chk[2];
  t_uint8	ack;

  do
    {
      /*
       * 1)
       */

      gdb_checksum(packet, chk);

      /*
       * 2)
       */

      serial_write(com_port, (t_uint8*)"$", 1);
      if (strlen((char*)packet))
	serial_write(com_port, packet, strlen((char*)packet));
      serial_write(com_port, (t_uint8*)"#", 1);
      serial_write(com_port, chk, 2);

      /*
       * 3)
       */

      serial_read(com_port, &ack, 1);
    }
  while (ack == '-');
}

/*
 * this is the command handler. it waits for commands on serial port.
 *
 * steps:
 *
 * 1) align on command's beginning.
 * 2) read the packet.
 * 3) read and check the packet checksum.
 * 4) write acknowledgment.
 * 5) displatch the command.
 */

t_error		gdb_command(void* ctx)
{
  t_uint8	start;
  t_uint8	cmd;
  t_uint8	stop;
  t_uint8	chk[2];
  t_uint8	chkcalc[2];
  t_uint8	buffer[60];
  t_uint32	i;

  while (1)
    {
      /*
       * 1)
       */

      serial_read(com_port, &start, 1);
      if (start != '$')
	continue;
      serial_read(com_port, &cmd, 1);

      /*
       * 2)
       */

      buffer[0] = cmd;
      i = 1;
      do
	{
	  serial_read(com_port, &stop, 1);
	  buffer[i++] = stop;
	}
      while (stop != '#');
      buffer[i - 1] = 0;

      /*
       * 3)
       */

      serial_read(com_port, chk, 2);
      gdb_checksum(buffer, chkcalc);
      if (chk[0] != chkcalc[0] || chk[1] != chkcalc[1])
	{
	  printf("bad checksum (%c%c != %c%c)\n", chk[0], chk[1],
		 chkcalc[0], chkcalc[1]);
	  serial_write(com_port, (t_uint8*)"-", 1);
	  continue;
	}

      /*
       * 4)
       */

      serial_write(com_port, (t_uint8*)"+", 1);

      /*
       * 5)
       */

      for (i = 0; commands[i].cmd; i++)
	if (commands[i].cmd == cmd)
	  break;
      if (commands[i].cmd)
	{
	  if (commands[i].fun(buffer, ctx))
	    break;
	}
      else
	{
	  gdb_send((t_uint8*)"");
	}
    }
  return (ERROR_NONE);
}

/*
 * this function fills a buffer with given bytes in hex format.
 */

static void	gdb_fill_reg(t_uint8*		buffer,
			     t_uint32		value,
			     t_uint32		len)
{
  t_uint8*	p = buffer;

  for (; p < buffer + len; p += 2)
    {
      p++;
      *p = (value & 0xF);
      if (*p >= 10)
	*p = 'a' + (*p - 10);
      else
	*p += '0';
      value = value >> 4;
      p--;
      *p = (value & 0xF);
      if (*p >= 10)
	*p = 'a' + (*p - 10);
      else
	*p += '0';
      value = value >> 4;
    }
}

/*
 * this function builds a register dump and sends it to the client.
 */

int			gdb_read_reg(t_uint8*		buffer,
				     void*		context)
{
  t_uint8		send[353];
  t_gdb_context*	ctx = context;

  memset(send, '0', sizeof(send));
  send[sizeof(send) - 1] = 0;
  gdb_fill_reg(send + 0, ctx->eax, 8);
  gdb_fill_reg(send + 8, ctx->ecx, 8);
  gdb_fill_reg(send + 16, ctx->edx, 8);
  gdb_fill_reg(send + 24, ctx->ebx, 8);
  gdb_fill_reg(send + 32, 0 /* ctx->esp*/, 8);
  gdb_fill_reg(send + 40, ctx->ebp, 8);
  gdb_fill_reg(send + 48, ctx->esi, 8);
  gdb_fill_reg(send + 56, ctx->edi, 8);
  gdb_fill_reg(send + 64, ctx->eip, 8);
  gdb_fill_reg(send + 72, ctx->eflags, 8);
  gdb_fill_reg(send + 80, ctx->cs, 8);
  gdb_fill_reg(send + 88, 0 /* ctx->ss*/, 8);
  gdb_fill_reg(send + 96, ctx->ds, 8);
  gdb_send(send);
  return 0;
}

/*
 * this function is not supported.
 */

int		gdb_write_reg(t_uint8*		buffer)
{
  gdb_send((t_uint8*)"");
  return 0;
}

/*
 * this function reads from memory and sends read bytes to the client.
 *
 * steps:
 *
 * 1) parse the input packet.
 * 2) read from memory.
 * 3) build and send the packet.
 */

int		gdb_read_mem(t_uint8*		buffer)
{
  t_uint8*	p;
  t_vaddr	addr;
  t_vsize	sz;
  union {
    t_uint8*	b;
    t_uint16*	h;
    t_uint32*	w;
  }		u;
  t_uint32	val;

  /*
   * 1)
   */

  buffer++;
  for (p = buffer; *p && *p != ','; p++)
    ;
  *p = 0;
  p++;
  addr = strtol((char*)buffer, NULL, 16);
  sz = strtol((char*)p, NULL, 16);

  /*
   * 2)
   */

  sz *= 2;
  if (sz == 8)
    {
      u.b = (t_uint8*)addr;
      val = *u.b;
    }
  if (sz == 4)
    {
      u.h = (t_uint16*)addr;
      val = *u.h;
    }
  if (sz == 2)
    {
      u.w = (t_uint32*)addr;
      val = *u.w;
    }

  /*
   * 3)
   */

  p = malloc(sz + 1);
  memset(p, '0', sz);
  p[sz] = 0;
  gdb_fill_reg(p, val, sz);
  gdb_send(p);
  free(p);
  return 0;
}

/*
 * this function is not supported.
 */

int		gdb_write_mem(t_uint8*		buffer)
{

  gdb_send((t_uint8*)"");
  return 0;
}

/*
 * this function enables single-step mode.
 */

int		gdb_step(t_uint8*		buffer)
{
  step = 1;
  return 1;
}

/*
 * this function restarts the execution since next breakpoint.
 */

int		gdb_continue(t_uint8*		buffer)
{
  step = 0;
  return 1;
}

/*
 * this function removes a breakpoint.
 *
 * steps:
 *
 * 1) parse the incoming packet.
 * 2) remove the breakpoint and send ack.
 */

int		gdb_unset_break(t_uint8*	buffer)
{
  t_uint8*	p;
  t_uint8*	addr;
  t_vaddr	pt;
  t_id		id;

  /*
   * 1)
   */

  buffer++;
  if (*buffer != '0')
    {
      gdb_send((t_uint8*)"");
      return 0;
    }
  addr = buffer + 2;
  for (p = addr; *p && *p != ','; p++)
    ;
  *p = 0;
  pt = strtol((char*)addr, NULL, 16);
  id = pt;

  /*
   * 2)
   */

  if (set_remove(br, id) != ERROR_NONE)
    gdb_send((t_uint8*)"E00");
  else
    gdb_send((t_uint8*)"OK");
  return 0;
}

/*
 * this function adds a breakpoint.
 *
 * steps:
 *
 * 1) parse the incoming packet.
 * 2) add the breakpoint and send ack.
 */

int		gdb_set_break(t_uint8*		buffer)
{
  t_uint8*	p;
  t_uint8*	addr;
  t_vaddr	pt;
  t_id		id;

  /*
   * 1)
   */

  buffer++;
  if (*buffer != '0')
    {
      gdb_send((t_uint8*)"");
      return 0;
    }
  addr = buffer + 2;
  for (p = addr; *p && *p != ','; p++)
    ;
  *p = 0;
  pt = strtol((char*)addr, NULL, 16);
  id = pt;

  /*
   * 2)
   */

  if (set_add(br, &id) != ERROR_NONE)
    gdb_send((t_uint8*)"E00");
  else
    gdb_send((t_uint8*)"OK");
  return 0;
}

/*
 * this function signales status. we are always in TRAP mode.
 */

int		gdb_status(t_uint8*		buffer)
{
  gdb_send((t_uint8*)"S05");
  return 0;
}

/*
 * this function initialize the gdb server.
 *
 * steps:
 *
 * 1) reserve a set for the breakpoints list.
 * 2) initialize serial port.
 * 3) hook the int1 (debug) trap.
 * 4) manually ask for the interruption.
 * 5) set the TF flag.
 */

t_error		gdb_init(void)
{
  /*
   * 1)
   */
  if (set_reserve(array, SET_OPT_SORT, 10, sizeof(t_id), &br) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  serial_init(com_port, SERIAL_BR57600, SERIAL_8N1, SERIAL_FIFO_8);

  /*
   * 3)
   */

  if (event_reserve(1, EVENT_FUNCTION, (u_event_handler)gdb_handler) !=
      ERROR_NONE)
    return (ERROR_UNKNOWN);

  printf("gdb server initialized\n");

  /*
   * 4)
   */

  step = 1;
  asm volatile("int $1");

  /*
   * 5)
   */

  asm volatile("pushf\n\t"
	       "orw $0x0100, 0(%esp)\n\t"
	       "popf");

  return (ERROR_NONE);
}

/*
 * this function disable gdb server.
 *
 * steps:
 *
 * 1) unset the TF flag.
 * 2) release the event handler.
 * 3) release the breakpoints list.
 */

t_error		gdb_clean(void)
{
  /*
   * 1)
   */

  asm volatile("pushf\n\t"
	       "andw $0xFEFF, 0(%esp)\n\t"
	       "popf");

  /*
   * 2)
   */

  if (event_release(1) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (set_release(br) != ERROR_NONE)
    return (ERROR_UNKNOWN);
}
