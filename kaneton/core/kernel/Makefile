#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/kernel/Makefile
#
# created       julien quintard   [sun jun 10 16:57:54 2007]
# updated       julien quintard   [tue jun 26 15:46:39 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

KERNEL_C		:=		kernel.c			

KERNEL_O		:=		$(KERNEL_C:.c=.o)

KERNEL_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kernel.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			dependencies $(_KERNEL_LO_)

$(_KERNEL_LO_):		$(KERNEL_O) $(_LIBC_A_)
	$(call env_remove,$(_KERNEL_LO_),)

	$(call env_archive,$(_KERNEL_LO_),$(KERNEL_O),)

clear:
	$(call env_remove,$(KERNEL_O),)

	$(call env_remove,$(_KERNEL_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(KERNEL_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(KERNEL_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
