##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/core/kaneton/debug/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:39:09 2006]
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

DEBUG_LO		:=		debug.lo

DEBUG_C			:=		cons.c				\
					debug.c				\
					serial.c

DEBUG_O			:=		$(DEBUG_C:.c=.o)

DEBUG_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/serial.h \
					$(_CORE_INCLUDE_DIR_)/kaneton/debug.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(DEBUG_LO):		$(DEBUG_O)
	$(call remove,$(DEBUG_LO),)

	$(call archive,$(DEBUG_LO),$(DEBUG_O),)

clear:
	$(call remove,$(DEBUG_O),)

	$(call remove,$(DEBUG_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(DEBUG_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(DEBUG_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
