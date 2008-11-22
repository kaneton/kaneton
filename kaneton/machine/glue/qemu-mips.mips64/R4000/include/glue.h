/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...ine/glue/qemu-mips.mips64/include/glue.h
 *
 * created       enguerrand raymond   [sun oct 12 18:47:31 2008]
 * updated       enguerrand raymond   [sun oct 12 19:18:37 2008]
 */

#ifndef GLUE_GLUE_H
#define GLUE_GLUE_H			1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * machdep transparent traps
 */

#define		machine_include(_manager_)				\
  //machine_include_##_manager_()

#define		machine_call(_manager_, _function_, _arguments_...)	\
  ERROR_NONE//machine_call_##_manager_(_function_, ##_arguments_)

#define		machine_data(_object_)					\
  //machine_data_##_object_()

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/architecture.h>
#include <platform/platform.h>

#include <glue/init.h>

#endif
