##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/libsun4u/traps/Makefile
##
## created       matthieu bucchianeri   [thu aug 10 16:01:50 2006]
## updated       matthieu bucchianeri   [mon aug 14 18:43:27 2006]
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

TRAPS_LO	:=		traps.lo

TRAPS_C		:=		trap.c					\
				handler.c

TRAPS_O		:=		$(TRAPS_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(TRAPS_LO):		$(TRAPS_O)
	$(call remove,$(TRAPS_LO),)

	$(call archive,$(TRAPS_LO),$(TRAPS_O),)

clear:
	$(call remove,$(TRAPS_O),)

	$(call remove,$(TRAPS_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(TRAPS_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
