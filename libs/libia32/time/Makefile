##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/libs/libia32/timer/Makefile
##
## created       matthieu bucchianeri   [fri jan  6 11:45:56 2006]
## updated       matthieu bucchianeri   [fri jan  6 11:50:10 2006]
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

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(TIME_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
