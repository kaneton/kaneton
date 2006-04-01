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

include			../../../env/.env.mk

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
	$(call remove,$(EVENT_LO),)

	$(call archive,$(EVENT_LO),$(EVENT_O),)

clear:
	$(call remove,$(EVENT_O),)

	$(call remove,$(EVENT_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(EVENT_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(EVENT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
