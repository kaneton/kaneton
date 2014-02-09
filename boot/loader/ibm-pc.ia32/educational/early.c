/*
 * license	kaneton license
 *
 * project	kaneton
 *
 * file		early.c
 *
 * created	julian pidancet   [Fri 11 Jan 2008 06:54:49 PM CET]
 * updated	julian pidancet   [Fri 11 Jan 2008 06:54:49 PM CET]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "loader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * ---------- functions -------------------------------------------------------
 */

char* strings[]  __attribute__ ((section(".early.data"))) =
{
  "Divide Error",
  "Double Fault",
  "Stack Segment Fault",
  "Global Protection",
  "Page Fault",
  " at address 0x"
};

void print_status(char *s, t_uint32 address) __attribute__ ((section(".early.text")));
void print_status(char *s, t_uint32 address)
{
  t_uint16 *video = (t_uint16 *)0xB8000;
  t_sint32 i;

  while (*s)
  {
    *video++ = (*s++) | (0x4F << 8);
  }

  s = strings[(sizeof (strings) / 4) - 1];

  while (*s)
  {
    *video++ = (*s++) | (0x4F << 8);
  }

  for (i = 7; i >= 0; i--)
  {
    char digit = (address >> (i * 4)) & 0xF;

    if (digit < 10)
      *video++ = (digit + '0') | (0x4F << 8);
    else
      *video++ = ((digit - 10) + 'a') | (0x4F << 8);
  }
}

#define EXN_DEF(name, string, addr_offset)				\
  void name (void) __attribute__ ((section(".early.text")));		\
  void name (void)							\
  {									\
    t_uint32 fault_addr;						\
    asm ("movl " #addr_offset "(%%esp), %0\n" : "=r" (fault_addr));	\
    print_status(string, fault_addr);					\
    for (;;)								\
      asm ("hlt");							\
  }

#define FUNC_LOW(func)	    ((t_uint16)((t_uint32)func & 0xFFFF))
#define FUNC_HIGH(func)	    ((t_uint16)(((t_uint32)func >> 16) & 0xFFFF))

EXN_DEF(de_exn, strings[0], 28);
EXN_DEF(df_exn, strings[1], 32);
EXN_DEF(ss_exn, strings[2], 32);
EXN_DEF(gp_exn, strings[3], 32);
EXN_DEF(pf_exn, strings[4], 32);

struct
{
  t_uint32 a : 16;
  t_uint32 b : 16;
  t_uint32 c :  8;
  t_uint32 d :  3;
  t_uint32 e :  1;
  t_uint32 f :  1;
  t_uint32 g :  2;
  t_uint32 h :  1;
  t_uint32 i : 16;
} __attribute__ ((packed)) exn_table[32] __attribute__ ((section(".early.data")));


static
const struct
{
  t_uint16 a;
  t_vaddr b;
  t_uint16 c;
} __attribute__ ((packed)) sysdesc =
{ sizeof (exn_table), (t_vaddr)exn_table, 0 };

#define FILL_TABLE(idx, name)						\
  exn_table[(idx)].a = FUNC_LOW(name);					\
  exn_table[(idx)].b = 0x8;						\
  exn_table[(idx)].d = 7;						\
  exn_table[(idx)].e = 1;						\
  exn_table[(idx)].h = 1;						\
  exn_table[(idx)].i = FUNC_HIGH(name)

void			faultcatcher_init(void)  __attribute__ ((section(".early.text")));
void			faultcatcher_init(void)
{
  FILL_TABLE(0, de_exn);
  FILL_TABLE(8, df_exn);
  FILL_TABLE(12, ss_exn);
  FILL_TABLE(13, gp_exn);
  FILL_TABLE(14, pf_exn);

  asm ("lidt %0" : : "m"(sysdesc));
}
