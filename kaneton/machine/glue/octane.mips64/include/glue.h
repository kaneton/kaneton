/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/lec_l/kanet...achine/glue/octane.mips64/include/glue.h
 *
 * created       laurent lec   [fri mar 28 17:18:12 2008]
 * updated       laurent lec   [fri mar 28 17:18:56 2008]
 */

#ifndef GLUE_GLUE_H
#define GLUE_GLUE_H			1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * machine transparent traps
 */

/* #define		machine_include(_manager_)				\ */
/*   machine_include_##_manager_() */

/* #define		machine_call(_manager_, _function_, _arguments_...)	\ */
/*   machine_call_##_manager_(_function_, ##_arguments_) */

/* #define		machine_data(_object_)					\ */
/*   machine_data_##_object_() */

#define machine_include(_manager_)

#define machine_call(_manager_, _function_, _arguments_...) STATUS_OK

#define machine_data(_object_)

/*
 * ---------- includes --------------------------------------------------------
 */

#define REGION_VMEM_MIN             PAGESZ
#define REGION_VMEM_MAX             0xffffffffU

#include <architecture/architecture.h>
#include <platform/platform.h>

#include <glue/init.h>

#endif
