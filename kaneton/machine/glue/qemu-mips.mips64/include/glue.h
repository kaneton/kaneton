/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...ue/qemu-mips.mips64/R4000/include/glue.h
 *
 * created       enguerrand raymond   [sun oct 12 18:47:31 2008]
 * updated       enguerrand raymond   [sun jan 25 14:44:12 2009]
 */

#ifndef GLUE_GLUE_H
#define GLUE_GLUE_H			1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * machine transparent traps
 */

#define		machine_include(_manager_)				\
  //machine_include_##_manager_()

#define		machine_call(_manager_, _function_, _arguments_...)	\
  STATUS_OK//machine_call_##_manager_(_function_, ##_arguments_)

#define		machine_data(_object_)					\
  //machine_data_##_object_()

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/architecture.h>
#include <platform/platform.h>

#include <glue/init.h>
#include <glue/region.h>

#endif
