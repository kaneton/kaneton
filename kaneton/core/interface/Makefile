##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/interface/Makefile
##
## created       julien quintard   [sat dec 10 13:50:17 2005]
## updated       julien quintard   [fri jun 23 21:17:52 2006]
##

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

INTERFACE_C		:=		interface.c			

INTERFACE_O		:=		$(INTERFACE_C:.c=.o)

INTERFACE_INCLUDE	:=		$(_CORE_INCLUDE_DIR_)/interface.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			dependencies $(_INTERFACE_LO_)

$(_INTERFACE_LO_):	$(INTERFACE_O) $(_LIBC_A_)
	$(call env_remove,$(_INTERFACE_LO_),)

	$(call env_archive,$(_INTERFACE_LO_),$(INTERFACE_O),)

clear:
	$(call env_remove,$(INTERFACE_O),)

	$(call env_remove,$(_INTERFACE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(INTERFACE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(INTERFACE_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
