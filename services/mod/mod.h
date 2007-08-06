/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/services/mod/mod.h
 *
 * created       matthieu bucchianeri   [mon aug  6 00:05:12 2007]
 * updated       matthieu bucchianeri   [mon aug  6 00:06:46 2007]
 */

#ifndef MOD_MOD_H
#define MOD_MOD_H

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libc.h>

#include "elf.h"

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * from init.h
 */

typedef struct
{
  char*				name;
  t_psize			size;
}				t_input;

typedef struct
{
  t_uint32			ninputs;
}				t_inputs;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	mod.c
 */

/*
 * mod.c
 */

t_error			mod_load_32(Elf32_Ehdr*		header,
				    char*		name);

t_error			mod_load_64(Elf64_Ehdr*		header,
				    char*		name);

void			mod_launch(t_inputs*	inputs);

void			mod_serve(void);

int			main(int argc, char** argv, char** envp);


/*
 * eop
 */


#endif
