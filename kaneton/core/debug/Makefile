##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/debug/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       julien quintard   [sat apr  1 23:40:18 2006]
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

DEBUG_C			:=		$(_CONSOLE_FILE_)		\
					debug.c				\
					serial.c

DEBUG_O			:=		$(DEBUG_C:.c=.o)

DEBUG_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/serial.h \
					$(_CORE_INCLUDE_DIR_)/debug.h

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
