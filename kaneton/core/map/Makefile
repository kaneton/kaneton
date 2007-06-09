##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/map/Makefile
##
## created       matthieu bucchianeri   [sun feb 26 13:05:52 2006]
## updated       matthieu bucchianeri   [sat may  5 18:43:35 2007]
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

MAP_LO			:=		map.lo

MAP_C			:=		map.c

MAP_O			:=		$(MAP_C:.c=.o)

MAP_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/map.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(MAP_LO):		$(MAP_O)
	$(call env_remove,$(MAP_LO),)

	$(call env_archive,$(MAP_LO),$(MAP_O),)

clear:
	$(call env_remove,$(MAP_O),)

	$(call env_remove,$(MAP_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(MAP_INCLUDE),)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(MAP_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
