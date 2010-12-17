/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/machine/include/machine.h
 *
 * created       julien quintard   [thu jun  7 12:46:25 2007]
 * updated       julien quintard   [fri dec 17 21:05:18 2010]
 */

#ifndef MACHINE_H
#define MACHINE_H			1

/*
 * ---------- information -----------------------------------------------------
 *
 * note that the machine's components, especially the architecture, are
 * supposed to provide the following meta definitions.
 *
 *   ___kaneton$endian, the architecture's endian being ENDIAN_LITTLE,
 *                      ENDIAN_BIG etc.
 *   ___kaneton$wordsz, the architecture's word size in bytes.
 *   ___kaneton$framesz, the physical memory unit i.e frame size in bytes.
 *   ___kaneton$pagesz, the virtual memory unit i.e page size in bytes.
 *
 * in addition, the kaneton environment should provide the following
 * definitions when building the microkernel and its servers:
 *
 *   ___kaneton, indicates that kaneton is the current kernel.
 *   ___kaneton$kernel, indicates that the currently running entity is
 *                      the kernel.
 *   ___kaneton$server, indicates that the currently running entity is
 *                      a server, being a driver, service or a guest.
 *   ___kaneton$driver, indicates that the currently running entity is
 *                      a driver i.e a server which has access to the hardware.
 *   ___kaneton$service, indicates that the currently running entity is
 *                       a service i.e a server offering some functionalities
 *                       other servers can request.
 *   ___kaneton$guest, indicates the the currently running entity is a
 *                     guest i.e a non-privileged server.
 *
 * some of these meta definitions are actually used by the core. for instance
 * the memory managers rely on both ___kaneton$framesz and ___kaneton$pagesz.
 *
 * [XXX:improvement] for now, the ___kaneton${kernel,driver,service,guest}
 *                   meta definitions are not provided! they should be
 *                   through the compilation process which knows what
 *                   it is building, which layout---i.e LD script---it is
 *                   using.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * escape
 */

#define MACHINE_ESCAPE(_format_, _arguments_...)			\
  {									\
    module_call(report, record,						\
		_format_ " (%s:%u)",					\
		##_arguments_, __FUNCTION__, __LINE__);			\
									\
    return (ERROR_KO);							\
  }

/*
 * leave
 */

#define MACHINE_LEAVE()							\
  {									\
    return (ERROR_OK);							\
  }

#endif
