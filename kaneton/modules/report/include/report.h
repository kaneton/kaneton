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
 * updated       julien quintard   [tue nov 23 11:00:12 2010]
 */

#ifndef MODULES_REPORT_REPORT_H
#define MODULES_REPORT_REPORT_H	1

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
}			m_report;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../report.c
 */

/*
 * ../report.c
 */

void			report_dump(void);

int			report_character(char			c);

void			report_attribute(t_uint8		attribute);

void			report_record(char*			fmt,
				      ...);

t_error			report_initialize(void);

t_error			report_clean(void);


/*
 * eop
 */

#endif
