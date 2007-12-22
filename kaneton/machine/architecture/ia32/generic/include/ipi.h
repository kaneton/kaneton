/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan.../architecture/ia32/generic/include/ipi.h
 *
 * created       matthieu bucchianeri   [sat dec 22 21:55:01 2007]
 * updated       matthieu bucchianeri   [sat dec 22 21:57:19 2007]
 */

#ifndef ARCHITECTURE_IPI_H
#define ARCHITECTURE_IPI_H

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * enumerate ipi possible destinations.
 */

typedef enum
  {
	ipi_all,
	ipi_all_but_me,
	ipi_cpu
  }	t_ia32_ipi_dest;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../ipi.c
 */

/*
 * ../ipi.c
 */

void			ia32_ipi_send_init(void);

void			ia32_ipi_send_startup(void);

void			ia32_ipi_send_vector(t_uint8		vector,
					     t_ia32_ipi_dest	dest,
					     i_cpu		cpu);

void			ia32_ipi_acknowledge(void);


/*
 * eop
 */

#endif
