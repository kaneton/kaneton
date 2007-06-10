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

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

CAPABILITY_C		:=		capability.c			

CAPABILITY_O		:=		$(CAPABILITY_C:.c=.o)

CAPABILITY_INCLUDE	:=		$(_CORE_INCLUDE_DIR_)/capability.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_CAPABILITY_LO_)

$(_CAPABILITY_LO_):	$(CAPABILITY_O)
	$(call env_remove,$(_CAPABILITY_LO_),)

	$(call env_archive,$(_CAPABILITY_LO_),$(CAPABILITY_O),)

clear:
	$(call env_remove,$(CAPABILITY_O),)

	$(call env_remove,$(_CAPABILITY_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(CAPABILITY_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(CAPABILITY_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
