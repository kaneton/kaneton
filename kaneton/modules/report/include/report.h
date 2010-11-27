/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../kaneton/modules/report/include/report.h
 *
 * created       julien quintard   [wed jun  6 16:25:44 2007]
 * updated       julien quintard   [fri nov 26 00:03:59 2010]
 */

#ifndef MODULES_REPORT_REPORT_H
#define MODULES_REPORT_REPORT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <library/library.h>

/*
 * ---------- structures ------------------------------------------------------
 */

/*
 * the module structure.
 */

typedef struct
{
  char			buffer[4096];
  t_uint32		offset;

  struct
  {
    t_printf_char_fn	character;
    t_printf_attr_fn	attribute;
  }			printf;
}			m_module_report;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../report.c
 */

/*
 * ../report.c
 */

void			module_report_dump(void);

int			module_report_character(char		c);

void			module_report_record(char*		fmt,
					     ...);

t_error			module_report_initialize(void);

t_error			module_report_clean(void);


/*
 * eop
 */

#endif
