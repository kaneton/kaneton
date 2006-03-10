##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/core/kaneton/timer/Makefile
##
## created       julien quintard   [sat dec 10 13:50:17 2005]
## updated       julien quintard   [fri mar 10 04:22:54 2006]
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

TIMER_LO		:=		timer.lo

TIMER_C			:=		timer.c

TIMER_O			:=		$(TIMER_C:.c=.o)

TIMER_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/timer.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(TIMER_LO):		$(TIMER_O)
	$(call remove,$(TIMER_LO),)

	$(call archive,$(TIMER_LO),$(TIMER_O),)

clear:
	$(call remove,$(TIMER_O),)

	$(call remove,$(TIMER_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(TIMER_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(TIMER_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
