#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          kaneton/kaneton/modules/debug/Makefile
#
# created       julio guerra   [ sun sep 8 13:33:50 2013]
# updated       julio guerra   [ sun sep 8 13:33:50 2013]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/debug

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

DEBUG_LO		:=		debug.lo

DEBUG_C			:=		debug.c       \
					serial.c      \
					stubs.c       \
					misc.c        \
					read.c        \
					write.c       \
					parse.c       \
					handler.c     \
					server.c

DEBUG_O			:=		$(DEBUG_C:.c=.o)

DEBUG_INCLUDE		:=		$(directory)/include/console.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(DEBUG_LO)

$(DEBUG_LO):		$(DEBUG_O)
	$(call env_remove,$(DEBUG_LO),)

	$(call env_archive,$(DEBUG_LO),$(DEBUG_O),)

clear:
	$(call env_remove,$(DEBUG_O),)

	$(call env_remove,$(DEBUG_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(DEBUG_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(DEBUG_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
