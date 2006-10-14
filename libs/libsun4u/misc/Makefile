##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/libsun4u/misc/Makefile
##
## created       matthieu bucchianeri   [thu aug 10 16:01:50 2006]
## updated       matthieu bucchianeri   [mon aug 14 18:44:01 2006]
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

MISC_LO			:=		misc.lo

MISC_C			:=		tick.c

MISC_O			:=		$(MISC_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(MISC_LO):		$(MISC_O)
	$(call remove,$(MISC_LO),)

	$(call archive,$(MISC_LO),$(MISC_O),)

clear:
	$(call remove,$(MISC_O),)

	$(call remove,$(MISC_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MISC_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
