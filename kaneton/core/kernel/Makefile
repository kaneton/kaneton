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
# updated       julien quintard   [thu jun 28 20:43:38 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kernel

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

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

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

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
