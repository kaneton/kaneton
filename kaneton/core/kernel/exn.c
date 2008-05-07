#include <kaneton.h>
#include <klibc.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * ---------- functions -------------------------------------------------------
 */

#define EXN_DEF(name, string)						\
  void name (void);							\
  void name (void) { printf(string "\n\tSpinning...\n");		\
    for (;;) asm ("hlt"); }

#define FUNC_LOW(func)	    ((t_uint16)((t_uint32)func & 0xFFFF))
#define FUNC_HIGH(func)	    ((t_uint16)(((t_uint32)func >> 16) & 0xFFFF))

EXN_DEF(de_exn, "Divide Error");
EXN_DEF(df_exn, "Double Fault");
EXN_DEF(ss_exn, "Stack Segment Fault");
EXN_DEF(gp_exn, "Global Protection");
EXN_DEF(pf_exn, "Page Fault");

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
} __attribute__ ((packed)) exn_table[32];

static const struct
{
  t_uint16 a;
  t_vaddr b;
  t_uint16 c;
} __attribute__ ((packed)) sysdesc =
{ sizeof (exn_table), (t_vaddr)exn_table, 0 };

#define FILL_TABLE(idx, name)						\
  exn_table[(idx)].a = FUNC_LOW(name);					\
  exn_table[(idx)].b = PMODE_GDT_CORE_CS << 3;				\
  exn_table[(idx)].d = 7;						\
  exn_table[(idx)].e = 1;						\
  exn_table[(idx)].h = 1;						\
  exn_table[(idx)].i = FUNC_HIGH(name)

void			faultcatcher_init(void)
{
  FILL_TABLE(0, de_exn);
  FILL_TABLE(8, df_exn);
  FILL_TABLE(12, ss_exn);
  FILL_TABLE(13, gp_exn);
  FILL_TABLE(14, pf_exn);

  asm ("lidt %0" : : "m"(sysdesc));
}


