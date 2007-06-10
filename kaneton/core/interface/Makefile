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

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

INTERFACE_LO		:=		interface.lo

INTERFACE_C		:=		interface.c			

INTERFACE_O		:=		$(INTERFACE_C:.c=.o)

INTERFACE_INCLUDE	:=		$(_CORE_INCLUDE_DIR_)/interface.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(INTERFACE_LO):		$(INTERFACE_O)
	$(call env_remove,$(INTERFACE_LO),)

	$(call env_archive,$(INTERFACE_LO),$(INTERFACE_O),)

clear:
	$(call env_remove,$(INTERFACE_O),)

	$(call env_remove,$(INTERFACE_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(INTERFACE_INCLUDE),)

dep:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(INTERFACE_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
