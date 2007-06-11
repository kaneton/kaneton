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

include			../../../environment/env.mk

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
	$(call env_remove,$(TRAPS_LO),)

	$(call env_archive,$(TRAPS_LO),$(TRAPS_O),)

clear:
	$(call env_remove,$(TRAPS_O),)

	$(call env_remove,$(TRAPS_LO),)

	$(call env_purge,)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TRAPS_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
