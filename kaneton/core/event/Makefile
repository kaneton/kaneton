##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/event/Makefile
##
## created       julien quintard   [sat dec 10 13:50:17 2005]
## updated       julien quintard   [sat apr  1 23:40:07 2006]
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

EVENT_LO		:=		event.lo

EVENT_C			:=		event.c

EVENT_O			:=		$(EVENT_C:.c=.o)

EVENT_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/event.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(EVENT_LO):		$(EVENT_O)
	$(call env_remove,$(EVENT_LO),)

	$(call env_archive,$(EVENT_LO),$(EVENT_O),)

clear:
	$(call env_remove,$(EVENT_O),)

	$(call env_remove,$(EVENT_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(EVENT_INCLUDE),)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(EVENT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
