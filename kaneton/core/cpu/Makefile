#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/cpu/Makefile
#
# created       julien quintard   [tue jun 26 15:43:35 2007]
# updated       julien quintard   [fri jun 29 08:35:30 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/cpu

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

CPU_C			:=		cpu.c

CPU_O			:=		$(CPU_C:.c=.o)

CPU_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/cpu.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_CPU_LO_)

$(_CPU_LO_):		$(CPU_O) $(_LIBC_A_)
	$(call env_remove,$(_CPU_LO_),)

	$(call env_archive,$(_CPU_LO_),$(CPU_O),)

clear:
	$(call env_remove,$(CPU_O),)

	$(call env_remove,$(_CPU_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(CPU_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(CPU_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
