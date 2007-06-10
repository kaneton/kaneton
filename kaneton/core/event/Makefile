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

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

EVENT_C			:=		event.c

EVENT_O			:=		$(EVENT_C:.c=.o)

EVENT_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/event.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_EVENT_LO_)

$(_EVENT_LO_):		$(EVENT_O)
	$(call env_remove,$(_EVENT_LO_),)

	$(call env_archive,$(_EVENT_LO_),$(EVENT_O),)

clear:
	$(call env_remove,$(EVENT_O),)

	$(call env_remove,$(_EVENT_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(EVENT_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(EVENT_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
