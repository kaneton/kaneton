##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/set/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       julien quintard   [sat jul  1 20:35:12 2006]
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

SET_LO			:=		set.lo

SET_C			:=		set.c				\
					set-array.c			\
					set-bpt.c			\
					set-ll.c			\
					set-pipe.c			\
					set-stack.c			

SET_O			:=		$(SET_C:.c=.o)

SET_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/set.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(SET_LO):		$(SET_O)
	$(call env_remove,$(SET_LO),)

	$(call env_archive,$(SET_LO),$(SET_O),)

clear:
	$(call env_remove,$(SET_O),)

	$(call env_remove,$(SET_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(SET_INCLUDE),)

dep:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(SET_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
