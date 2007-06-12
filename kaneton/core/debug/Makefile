##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/crypt/kaneton/kaneton/core/debug/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       matthieu bucchianeri   [tue feb 13 14:51:08 2007]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

DEBUG_C			:=		$(_CONSOLE_FILE_)		\
					debug.c				\
					gdb.c				\
					serial.c

DEBUG_O			:=		$(DEBUG_C:.c=.o)

DEBUG_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/serial.h \
					$(_CORE_INCLUDE_DIR_)/debug.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_DEBUG_LO_)

$(_DEBUG_LO_):		$(DEBUG_O)
	$(call env_remove,$(_DEBUG_LO_),)

	$(call env_archive,$(_DEBUG_LO_),$(DEBUG_O),)

clear:
	$(call env_remove,$(DEBUG_O),)

	$(call env_remove,$(_DEBUG_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(DEBUG_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(DEBUG_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
