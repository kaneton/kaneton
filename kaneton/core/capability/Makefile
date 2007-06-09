##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/capability/Makefile
##
## created       julien quintard   [sat dec 10 13:50:17 2005]
## updated       julien quintard   [fri jun 23 21:12:33 2006]
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

CAPABILITY_LO		:=		capability.lo

CAPABILITY_C		:=		capability.c			

CAPABILITY_O		:=		$(CAPABILITY_C:.c=.o)

CAPABILITY_INCLUDE	:=		$(_CORE_INCLUDE_DIR_)/capability.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(CAPABILITY_LO):		$(CAPABILITY_O)
	$(call env_remove,$(CAPABILITY_LO),)

	$(call env_archive,$(CAPABILITY_LO),$(CAPABILITY_O),)

clear:
	$(call env_remove,$(CAPABILITY_O),)

	$(call env_remove,$(CAPABILITY_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(CAPABILITY_INCLUDE),)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(CAPABILITY_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
