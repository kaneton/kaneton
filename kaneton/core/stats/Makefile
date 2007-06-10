##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/stats/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       julien quintard   [sat apr  1 23:39:11 2006]
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

STATS_LO		:=		stats.lo

STATS_C			:=		stats.c

STATS_O			:=		$(STATS_C:.c=.o)

STATS_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/stats.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(STATS_LO):		$(STATS_O)
	$(call env_remove,$(STATS_LO),)

	$(call env_archive,$(STATS_LO),$(STATS_O),)

clear:
	$(call env_remove,$(STATS_O),)

	$(call env_remove,$(STATS_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(STATS_INCLUDE),)

dep:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(STATS_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
