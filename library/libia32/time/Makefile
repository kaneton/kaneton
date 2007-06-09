##
## licence       Kaneton licence
##
## project       kaneton
##
## file          /home/rhino/kaneton/libs/libia32/time/Makefile
##
## created       renaud voltz   [mon mar 20 06:24:36 2006]
## updated       renaud voltz   [mon mar 20 06:24:36 2006]
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

TIME_LO			:=		time.lo

TIME_C			:=		timer.c

TIME_O			:=		$(TIME_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(TIME_LO):		$(TIME_O)
	$(call env_remove,$(TIME_LO),)

	$(call env_archive,$(TIME_LO),$(TIME_O),)

clear:
	$(call env_remove,$(TIME_O),)

	$(call env_remove,$(TIME_LO),)

	$(call env_purge,)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(TIME_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
