/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/debug/gdb.c
 *
 * created       matthieu bucchianeri   [mon apr 10 12:47:20 2006]
 * updated       matthieu bucchianeri   [mon apr 10 20:05:04 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- types -----------------------------------------------------------
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
  t_uint8	reserved3[0x18];
  t_uint32	reserved2;
  t_uint32	ebp;
  t_uint32	eip;
  t_uint32	cs;
  t_uint32	eflags;
}		__attribute__ ((packed)) t_gdb_context;

typedef int (*t_pfngdb)();

/*
 * ---------- globals ---------------------------------------------------------
 */

static const t_uint32		com_port = SERIAL_COM1;
static volatile t_uint32	step = 0;

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
//    { 'z', "unset breakpoint", gdb_unset_break },
//    { 'Z', "set breakpoint", gdb_set_break },
    { '?', "status ?", gdb_status },
    { 0, 0, 0 }
  };

/*
 * ---------- functions -------------------------------------------------------
 */

void			gdb_handler(t_uint32 needless)
{
  t_gdb_context*	ctx;
  t_uint8*		ptr;

  asm volatile("movl (%%ebp), %%eax\n\t"
	       "movl (%%eax), %%ebx\n\t"
	       "movl %%ebx, %0"
	       : "=g" (ptr)
	       :
	       : "%eax", "%ebx");

  ptr -= 60;
  ctx = (t_gdb_context*)ptr;
  if (step)
    gdb_command(ctx);
}

t_error		gdb_init(void)
{
  serial_init(com_port, SERIAL_BR57600, SERIAL_8N1, SERIAL_FIFO_8);
  if (event_reserve(1, EVENT_FUNCTION, (u_event_handler)gdb_handler) !=
      ERROR_NONE)
    printf("cannot setup int1\n");

  printf("gdb server initialized\n");

  step = 1;
  asm volatile("int $1");

  asm volatile("pushf\n\t"
	       "orw $0x0100, 0(%esp)\n\t"
	       "popf");

  return (ERROR_NONE);
}

void		gdb_checksum(t_uint8*		packet,
			     t_uint8*		chksum)
{
  t_uint32	sum = 0;

  for (; *packet; packet++)
    sum += *packet;
  sum = sum % 256;
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

void		gdb_send(t_uint8*		packet)
{
  t_uint8	chk[2];
  t_uint8	ack;

  do
    {
      gdb_checksum(packet, chk);
      serial_write(com_port, "$", 1);
      if (strlen(packet))
	serial_write(com_port, packet, strlen(packet));
      serial_write(com_port, "#", 1);
      serial_write(com_port, chk, 2);
      serial_read(com_port, &ack, 1);
    }
  while (ack == '-');
}

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
      serial_read(com_port, &start, 1);
      if (start != '$')
	continue;
      serial_read(com_port, &cmd, 1);
      buffer[0] = cmd;
      i = 1;
      do
	{
	  serial_read(com_port, &stop, 1);
	  buffer[i++] = stop;
	}
      while (stop != '#');
      buffer[i - 1] = 0;
      serial_read(com_port, chk, 2);
      gdb_checksum(buffer, chkcalc);
      if (chk[0] != chkcalc[0] || chk[1] != chkcalc[1])
	{
	  printf("bad checksum (%c%c != %c%c)\n", chk[0], chk[1],
		 chkcalc[0], chkcalc[1]);
	  serial_write(com_port, "-", 1);
	  continue;
	}
      serial_write(com_port, "+", 1);
      for (i = 0; commands[i].cmd; i++)
	if (commands[i].cmd == cmd)
	  break;
      if (commands[i].cmd)
	{
	  printf("command: %s\n", commands[i].desc);
	  if (commands[i].fun(buffer, ctx))
	    break;
	}
      else
	{
	  printf("unknown command: %s\n", buffer);
	  gdb_send("");
	}
    }
  return (ERROR_NONE);
}

void		gdb_fill_reg(t_uint8*		buffer,
			     t_uint32		value)
{
  t_uint8*	p = buffer;

  for (; p < buffer + 8; p += 2)
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

int			gdb_read_reg(t_uint8*		buffer,
				     void*		context)
{
  t_uint8		send[353];
  t_gdb_context*	ctx = context;

  memset(send, '0', sizeof(send));
  send[sizeof(send) - 1] = 0;
  gdb_fill_reg(send + 0, ctx->eax);
  gdb_fill_reg(send + 8, ctx->ecx);
  gdb_fill_reg(send + 16, ctx->edx);
  gdb_fill_reg(send + 24, ctx->ebx);
  gdb_fill_reg(send + 28, 0 /* ctx->esp*/);
  gdb_fill_reg(send + 32, ctx->ebp);
  gdb_fill_reg(send + 36, ctx->esi);
  gdb_fill_reg(send + 40, ctx->edi);
  gdb_fill_reg(send + 44, ctx->eip);
  gdb_fill_reg(send + 48, ctx->eflags);
  gdb_fill_reg(send + 52, ctx->cs);
  gdb_fill_reg(send + 56, 0 /* ctx->ss*/);
  gdb_fill_reg(send + 60, ctx->ds);
  /* XXX */
  gdb_send(send);
  return 0;
}

int		gdb_write_reg(t_uint8*		buffer)
{
  gdb_send("");
  return 0;
}

int		gdb_read_mem(t_uint8*		buffer)
{
  t_uint8*	p;
  t_vaddr	addr;
  t_vsize	sz;

  for (p = buffer; *p && *p != ','; p++)
    ;
  *p = 0;
  p++;
  addr = strtol(buffer, NULL, 10);
  sz = strtol(p, NULL, 10);
//  printf("reading %d bytes @ %d\n", sz, addr);
  p = malloc(sz + 1);
  memset(p, '0', sz);
  p[sz] = 0;
  gdb_send(p);
  free(p);
  return 0;
}

int		gdb_write_mem(t_uint8*		buffer)
{
  gdb_send("");
  return 0;
}

int		gdb_step(t_uint8*		buffer)
{
  step = 1;
  return 1;
}

int		gdb_continue(t_uint8*		buffer)
{
  return 1;
}

int		gdb_unset_break(t_uint8*	buffer)
{
  return 0;
}

int		gdb_set_break(t_uint8*		buffer)
{
  return 0;
}

int		gdb_status(t_uint8*		buffer)
{
  gdb_send("S05");
  return 0;
}
