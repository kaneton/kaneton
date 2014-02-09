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
 * updated       julien quintard   [sun dec 19 12:04:58 2010]
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
 *
 *
 * in addition, the machine must declare a certain number of types which
 * are listed below:
 *
 *   t_uint8, an unsigned 8-bit type.
 *   t_sint8, a signed 8-bit type.
 *   t_uint16, an unsigned 16-bit type.
 *   t_sint16, a signed 16-bit type
 *   t_uint32, an unsigned 32-bit type.
 *   t_sint32, a signed 32-bit type.
 *   t_uint64, an unsigned 64-bit type
 *   t_sint64, a signed 64-bit type
 *   t_reg8, an 8-bit register.
 *   t_reg16, a 16-bit register.
 *   t_reg32, a 32-bit register.
 *   t_reg64, a 64-bit register.
 *
 * but also:
 *
 *   t_paddr, an address in the physical memory space.
 *   t_psize, the size of an area of physical memory.
 *   t_vaddr, an address in the virtual memory space.
 *   t_vsize, the size of an area of virtual memory.
 *
 * [XXX:improvement] for now, the ___kaneton${kernel,driver,service,guest}
 *                   meta definitions are not provided! they should be
 *                   through the compilation process which knows what
 *                   it is building, which layout---i.e LD script---it is
 *                   using.
 */

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * the machine_include() is used to include in the core definitions provided
 * by the machine such as the dispatchers' definitions for instance.
 */

#define		machine_include(_manager_)				\
  machine_include_ ## _manager_()

/*
 * the machine_call() macro-function enables a function to call its
 * machine counterpart, should the platform, architecture or both need to
 * perform hardware operations. note that kaneton has been designed around
 * this portable scheme. however a few rules apply, especially regarding
 * the placement of these machine_call()s. indeed, should the object on which
 * the operation applies be a creation or modification, the machine_call()
 * must be placed at the end of the core's function, once everything has
 * been performed by the core, with a few exceptions i.e if one of the core's
 * actions may prevent the machine from performing its part. for an example,
 * please refer to thread_sleep() for example. should the object be removed
 * however, the machine should be called prior to performing anything in
 * the core as the machine may need to undo was has been done at the
 * object's creation. for more information, the reader is advised to browser
 * through the core's managers in order to see when the machine is called.
 */

#define		machine_call(_manager_, _function_, _arguments_...)	\
  machine_call_ ## _manager_(_function_, ##_arguments_)

/*
 * finally, machine_data() is used for including machine-specific information
 * within core managers, objects, structures etc. indeed, for example, on
 * the IA32 architecture, every address space is associated with a so-called
 * page directory. the address of this page directory must be kept somewhere
 * so that the machine can perform its machine-specific adjustments whenever
 * a memory management operation is requested. although the machine could
 * maintain data structures linking an address space identifier to its
 * page directory, it is far easier to include the page directory's address
 * directly in the address space object.
 */

#define		machine_data(_object_)					\
  machine_data_ ## _object_()

/*
 * escape
 */

#define MACHINE_ESCAPE(_format_, _arguments_...)			\
  {									\
    module_call(report, record,						\
		_format_ " (%s:%u)",					\
		##_arguments_, __FUNCTION__, __LINE__);			\
									\
    return (STATUS_ERROR);							\
  }

/*
 * leave
 */

#define MACHINE_LEAVE()							\
  {									\
    return (STATUS_OK);							\
  }

/*
 * ---------- includes --------------------------------------------------------
 */

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

#endif
