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
 * updated       julien quintard   [sat dec  4 23:00:06 2010]
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

void			module_report_character(char		c);

void			module_report_attribute(t_uint8		attribute);

void			module_report_record(char*		fmt,
					     ...);

t_error			module_report_load(void);

t_error			module_report_unload(void);


/*
 * eop
 */

#endif
