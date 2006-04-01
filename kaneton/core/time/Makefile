##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/time/Makefile
##
## created       julien quintard   [sat dec 10 13:50:17 2005]
## updated       julien quintard   [sat apr  1 23:37:52 2006]
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

TIME_LO			:=		time.lo

TIME_C			:=		timer.c

TIME_O			:=		$(TIME_C:.c=.o)

TIME_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/timer.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(TIME_LO):		$(TIME_O)
	$(call remove,$(TIME_LO),)

	$(call archive,$(TIME_LO),$(TIME_O),)

clear:
	$(call remove,$(TIME_O),)

	$(call remove,$(TIME_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(TIME_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(TIME_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
