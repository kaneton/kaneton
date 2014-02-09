/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/lec_l/kanet...achine/glue/octane.mips64/include/glue.h
 *
 * created       julien quintard   [mon jun 11 05:57:15 2007]
 * updated       laurent lec   [wed mar 26 18:14:09 2008]
 */

#ifndef GLUE_GLUE_H
#define GLUE_GLUE_H			1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define machine_include(_manager_)

#define machine_call(_manager_, _function_, _arguments_...)		\
  STATUS_OK

#define machine_data(_object_)

#endif
