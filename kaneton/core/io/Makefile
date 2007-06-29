#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/io/Makefile
#
# created       julien quintard   [thu jun 28 20:33:15 2007]
# updated       julien quintard   [fri jun 29 08:36:08 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/io

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

IO_C			:=		io.c

IO_O			:=		$(IO_C:.c=.o)

IO_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/io.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_IO_LO_)

$(_IO_LO_):		$(IO_O) $(_LIBC_A_)
	$(call env_remove,$(_IO_LO_),)

	$(call env_archive,$(_IO_LO_),$(IO_O),)

clear:
	$(call env_remove,$(IO_O),)

	$(call env_remove,$(_IO_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(IO_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(IO_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
