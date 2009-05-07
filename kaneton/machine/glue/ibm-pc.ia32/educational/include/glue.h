/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/ibm-pc.ia32/educational/include/glue.h
 *
 * created       julien quintard   [mon jun 11 05:57:15 2007]
 * updated       julien quintard   [fri may  1 00:53:23 2009]
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
  machine_include_##_manager_()

#define		machine_call(_manager_, _function_, _arguments_...)	\
  machine_call_##_manager_(_function_, ##_arguments_)

#define		machine_data(_object_)					\
  machine_data_##_object_()

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/architecture.h>
#include <platform/platform.h>

#include <glue/as.h>
#include <glue/cpu.h>
#include <glue/event.h>
#include <glue/init.h>
#include <glue/io.h>
#include <glue/message.h>
#include <glue/region.h>
#include <glue/scheduler.h>
#include <glue/segment.h>
#include <glue/task.h>
#include <glue/thread.h>
#include <glue/timer.h>

#endif
