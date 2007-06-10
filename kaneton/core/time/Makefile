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

TIME_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/timer.h

CFLAGS			+=		-D___kernel

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

proto:
	$(call env_prototypes,$(TIME_INCLUDE),)

dep:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TIME_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
