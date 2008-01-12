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

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * ---------- functions -------------------------------------------------------
 */

void print_string(char *s) __attribute__ ((section(".early.text")));
void print_string(char *s)
{
  t_uint16 *video = (t_uint16 *)0xB8000;

  while (*s)
  {
    *video++ = (*s++) | (0x4F << 8);
  }
}

#define EXN_DEF(name, string)						\
  void name (void) __attribute__ ((section(".early.text")));		\
  void name (void) { print_string(string);				\
    for (;;) asm ("hlt"); }

#define FUNC_LOW(func)	    ((t_uint16)((t_uint32)func & 0xFFFF))
#define FUNC_HIGH(func)	    ((t_uint16)(((t_uint32)func >> 16) & 0xFFFF))

char* strings[]  __attribute__ ((section(".early.data"))) =
{
  "Divide Error",
  "Double Fault",
  "Stack Segment Fault",
  "Global Protection",
  "Page Fault"
};

EXN_DEF(de_exn, strings[0]);
EXN_DEF(df_exn, strings[1]);
EXN_DEF(ss_exn, strings[2]);
EXN_DEF(gp_exn, strings[3]);
EXN_DEF(pf_exn, strings[4]);

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

