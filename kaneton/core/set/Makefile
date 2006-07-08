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

include			../../../env/.env.mk

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
	$(call remove,$(SET_LO),)

	$(call archive,$(SET_LO),$(SET_O),)

clear:
	$(call remove,$(SET_O),)

	$(call remove,$(SET_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(SET_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(SET_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
