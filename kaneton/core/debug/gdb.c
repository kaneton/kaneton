/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/debug/gdb.c
 *
 * created       matthieu bucchianeri   [mon apr 10 12:47:20 2006]
 * updated       matthieu bucchianeri   [sun may  6 18:38:48 2007]
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
 * ---------- defines ---------------------------------------------------------
 */

#define	QUIET

/*
 * activating int 1 is slower.
 */

//#define	INT1

/*
 * otherwise the default is int 3.
 */

#ifndef INT1
# define INT3
#endif /* !INT1 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#ifdef GDB_STUB

/*
 * ---------- types -----------------------------------------------------------
 */

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
static volatile t_uint32	step_old_opcode = 0;
static volatile t_uint32	step_old_eip = 0;
static volatile t_vaddr		ebp = 0;

/*
 * the breakpoints addresses set.
 */

static i_set			br = ID_UNUSED;

/*
 * breakpoint.
 */

typedef struct
{
  t_id		id;
  t_uint32	opcode;
}		t_brk;

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
 * 1) stops the execution if needed.
 */

void			gdb_handler(t_uint32 needless)
{
  t_brk*		brk;
  t_uint32		bp = 0;

  /*
   * 1)
   */

#ifdef INT1
  if (set_get(br, context->eip, (void**)&brk) == ERROR_NONE)
#else
  if (set_get(br, (context->eip - 1), (void**)&brk) == ERROR_NONE)
#endif /* INT1 */
    {
      bp = 1;
#ifdef INT3
      context->eip--;
#ifndef QUIET
      printf("placing back opcode @ %p (br)\n", context->eip);
#endif /* !QUIET */
      *(t_uint32*)(context->eip) = brk->opcode;
#endif /* INT3 */
    }

  if (step && step_old_opcode)
    {
#ifdef INT3
#ifndef QUIET
      printf("placing back opcode @ %p\n", step_old_eip);
#endif /* !QUIET */
      *(t_uint32*)step_old_eip = step_old_opcode;
#endif /* INT3 */
    }

  if ((step/* && context->ebp == ebp*/) ||
      ebp == 0 ||
      bp)
    {
      gdb_status(NULL);
      step = 0;
      gdb_command();
    }

  if (!bp && step && step_old_opcode)
    {
#ifdef INT3
      context->eip = step_old_eip;
#ifndef QUIET
      printf("resuming at %p\n", context->eip);
#endif /* !QUIET */
#endif /* INT3 */
    }

  if (bp)
    {
#ifdef INT3
#ifndef QUIET
      printf("resuming at %p\n", context->eip);
#endif /* !QUIET */
#endif /* INT3 */
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

t_error		gdb_command(void)
{
  t_uint8	start;
  t_uint8	cmd;
  t_uint8	stop;
  t_uint8	chk[2];
  t_uint8	chkcalc[2];
  t_uint8	buffer[400];
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
	  printf("cmd = %s\n", buffer);
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
	  if (commands[i].fun(buffer))
	    break;
	}
      else
	{
	  gdb_send((t_uint8*)"");
	}
    }
  ebp = context->ebp;
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
 * extract some hex data.
 */

t_uint32		gdb_extract(t_uint8*	buffer,
				    t_uint32	sz)
{
  t_uint8		c;
  t_uint32		val;

  c = *(buffer + sz);
  *(buffer + sz) = 0;
  val = strtol((char*)buffer, NULL, 16);
  *(buffer + sz) = c;
  return val;
}

/*
 * this function builds a register dump and sends it to the client.
 */

int			gdb_read_reg(t_uint8*		buffer)
{
  t_uint8		send[353];

  memset(send, '0', sizeof(send));
  send[sizeof(send) - 1] = 0;
  gdb_fill_reg(send + 0, context->eax, 8);
  gdb_fill_reg(send + 8, context->ecx, 8);
  gdb_fill_reg(send + 16, context->edx, 8);
  gdb_fill_reg(send + 24, context->ebx, 8);
  gdb_fill_reg(send + 32, context->esp, 8);
  gdb_fill_reg(send + 40, context->ebp, 8);
  gdb_fill_reg(send + 48, context->esi, 8);
  gdb_fill_reg(send + 56, context->edi, 8);
  gdb_fill_reg(send + 64, context->eip, 8);
  gdb_fill_reg(send + 72, context->eflags, 8);
  gdb_fill_reg(send + 80, context->cs, 8);
  gdb_fill_reg(send + 88, context->ss, 8);
  gdb_fill_reg(send + 96, context->ds, 8);
  gdb_send(send);
  return 0;
}

/*
 * this function is not supported.
 */

int		gdb_write_reg(t_uint8*		buffer)
{
  buffer++;
  context->eax = gdb_extract(buffer + 0, 8);
  context->ecx = gdb_extract(buffer + 8, 8);
  context->edx = gdb_extract(buffer + 16, 8);
  context->ebx = gdb_extract(buffer + 24, 8);
  context->esp = gdb_extract(buffer + 32, 8);
  context->ebp = gdb_extract(buffer + 40, 8);
  context->esi = gdb_extract(buffer + 48, 8);
  context->edi = gdb_extract(buffer + 56, 8);
  context->eip = gdb_extract(buffer + 64, 8);
  context->eflags = gdb_extract(buffer + 72, 8);
  context->cs = gdb_extract(buffer + 80, 8);
  context->ss = gdb_extract(buffer + 88, 8);
  context->ds = gdb_extract(buffer + 96, 8);
  gdb_send((t_uint8*)"OK");
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
  if (sz == 2)
    {
      u.b = (t_uint8*)addr;
      val = *u.b;
    }
  if (sz == 4)
    {
      u.h = (t_uint16*)addr;
      val = *u.h;
    }
  if (sz == 8)
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
  t_uint8*	p;
  t_uint8*	p2;
  t_vaddr	addr;
  t_vsize	sz;
  union {
    t_uint8*	b;
    t_uint16*	h;
    t_uint32*	w;
  }		u;
  t_uint32	val;
  t_uint32	val2;

  /*
   * 1)
   */

  buffer++;
  for (p = buffer; *p && *p != ','; p++)
    ;
  *p = 0;
  p++;
  for (p2 = p; *p2 && *p2 != ':'; p2++)
    ;
  *p2 = 0;
  p2++;
  addr = strtol((char*)buffer, NULL, 16);
  sz = strtol((char*)p, NULL, 16);
  if (sz == 8)
    {
      t_uint8 c;

      c = *(p2 + 8);
      *(p2 + 8) = 0;
      val = strtol((char*)p2, NULL, 16);
      *(p2 + 8) = c;
      val2 = strtol((char*)p2 + 8, NULL, 16);
    }
  else
    val = strtol((char*)p2, NULL, 16);

  /*
   * 2)
   */

  sz *= 2;
  if (sz == 2)
    {
      u.b = (t_uint8*)addr;
      *u.b = val;
    }
  if (sz == 4)
    {
      u.h = (t_uint16*)addr;
      *u.h = val;
    }
  if (sz == 8)
    {
      u.w = (t_uint32*)addr;
      *u.w = val;
    }
  if (sz == 16)
    {
      u.w = (t_uint32*)addr;
      *u.w = val;
      u.w = (t_uint32*)addr + 1;
      *u.w = val2;
    }
  gdb_send((t_uint8*)"OK");
  return 0;
}

/*
 * this function enables single-step mode.
 *
 * steps:
 *
 * 1) parse the command.
 * 2) place an int3.
 */

int			gdb_step(t_uint8*		buffer)
{
  t_vaddr		addr;
  t_uint32		eip;

  /*
   * 1)
   */

  buffer++;
  if (*buffer)
    {
      addr = strtol((char*)buffer, NULL, 16);
      context->eip = addr;
    }
  step = 1;

  /*
   * 2)
   */

#ifdef INT3
  if ((t_uint8)step_old_opcode == 0xc3)
    {
      eip = *(t_uint32*)(ebp + 4);
      step_old_eip = eip;
      step_old_opcode = *(t_uint32*)eip;
#ifndef QUIET
      printf("placing INT3 @ %p (after RET)\n", eip);
#endif /* !QUIET */
      *(t_uint8*)eip = 0xcc;
    }
  else
    {
      step_old_eip = context->eip;
      step_old_opcode = *(t_uint32*)context->eip;
#ifndef QUIET
      printf("placing INT3 @ %p\n", context->eip);
#endif /* !QUIET */
      *(t_uint8*)context->eip = 0xcc;
    }
#endif /* INT3 */

  return 1;
}

/*
 * this function restarts the execution since next breakpoint.
 */

int		gdb_continue(t_uint8*		buffer)
{
  t_vaddr		addr;

  buffer++;
  if (*buffer)
    {
      addr = strtol((char*)buffer, NULL, 16);
      context->eip = addr;
    }
  step = 0;
  return 1;
}

/*
 * this function removes a breakpoint.
 *
 * steps:
 *
 * 1) parse the incoming packet.
 * 2) remove the int3 instruction.
 * 3) remove the breakpoint and send ack.
 */

int		gdb_unset_break(t_uint8*	buffer)
{
  t_uint8*	p;
  t_uint8*	addr;
  t_vaddr	pt;
  t_brk*	brk;

  /*
   * 1)
   */

  buffer++;
  if (*buffer != '0')
    {
      printf("unsupported breakpoint type\n");
      gdb_send((t_uint8*)"");
      return 0;
    }
  addr = buffer + 2;
  for (p = addr; *p && *p != ','; p++)
    ;
  *p = 0;
  pt = strtol((char*)addr, NULL, 16);

  /*
   * 2)
   */

#ifdef INT3
  if (set_get(br, pt, (void**)&brk) != ERROR_NONE)
    {
      gdb_send((t_uint8*)"E00");
      return 0;
    }
  *(t_uint32*)((t_uint32)brk->id) = brk->opcode;
#endif /* INT3 */


  /*
   * 3)
   */

  if (set_remove(br, pt) != ERROR_NONE)
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
 * 2) add a int3 instruction.
 * 3) add the breakpoint and send ack.
 */

int		gdb_set_break(t_uint8*		buffer)
{
  t_uint8*	p;
  t_uint8*	addr;
  t_vaddr	pt;
  t_brk		brk;

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

  brk.id = pt;
  brk.opcode = *(t_uint32*)pt;

  /*
   * 2)
   */

#ifdef INT3
#ifndef QUIET
  printf("placing INT3 @ %p\n", pt);
#endif /* !QUIET */
  *(t_uint8*)pt = 0xcc;
#endif /* INT3 */

  /*
   * 3)
   */

  if (set_add(br, &brk) != ERROR_NONE)
    gdb_send((t_uint8*)"E00");
  else
    gdb_send((t_uint8*)"OK");
  return 0;
}

/*
 * this function signales status. we are always in TRAP mode.
 */

int			gdb_status(t_uint8*		buffer)
{
  t_uint8		buf[60];

  if (buffer)
    {
      memset(buf, 0, 60);
      strcpy((char*)buf, "T05");
      buf[3] = '8';
      buf[4] = ':';
      gdb_fill_reg(buf + 5, context->eip, 8);
      buf[13] = ';';
      buf[14] = 0;
      gdb_send(buf);
    }
  else
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
  if (set_reserve(array, SET_OPT_SORT | SET_OPT_ALLOC, 10,
		  sizeof(t_brk), &br) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  serial_init(com_port, SERIAL_BR57600, SERIAL_8N1, SERIAL_FIFO_8);

  /*
   * 3)
   */

  if (event_reserve(1, EVENT_FUNCTION, EVENT_HANDLER(gdb_handler)) !=
      ERROR_NONE ||
      event_reserve(3, EVENT_FUNCTION, EVENT_HANDLER(gdb_handler)) !=
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

#ifdef INT1
  asm volatile("pushf\n\t"
	       "orw $0x0100, 0(%esp)\n\t"
	       "popf");
#endif

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

  if (event_release(1) != ERROR_NONE ||
      event_release(3) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (set_release(br) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}
#endif
