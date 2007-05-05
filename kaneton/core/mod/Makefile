##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/mod/Makefile
##
## created       matthieu bucchianeri   [sat may  5 18:41:32 2007]
## updated       matthieu bucchianeri   [sat may  5 18:42:08 2007]
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

MOD_LO			:=		mod.lo

MOD_C			:=		mod.c


MOD_O			:=		$(MOD_C:.c=.o)

MOD_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/mod.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(MOD_LO):		$(MOD_O)
	$(call remove,$(MOD_LO),)

	$(call archive,$(MOD_LO),$(MOD_O),)

clear:
	$(call remove,$(MOD_O),)

	$(call remove,$(MOD_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(MOD_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MOD_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
