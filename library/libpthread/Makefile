#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/library/libpthread/Makefile
#
# created       matthieu bucchianeri   [thu sep  6 21:56:28 2007]
# updated       matthieu bucchianeri   [thu sep  6 22:33:40 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		libpthread

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

LIBPTHREAD_C		:=		thread.c		\
					cond.c			\
					mutex.c			\
					barrier.c

LIBPTHREAD_O		:=		$(LIBPTHREAD_C:.c=.o)

LIBPTHREAD_INCLUDE	:=		$(_LIBPTHREAD_INCLUDE_DIR_)/pthread.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_LIBPTHREAD_A_)

$(_LIBPTHREAD_A_):		$(LIBPTHREAD_O)
	$(call env_static-library,$(_LIBPTHREAD_A_),$(LIBPTHREAD_O),)

clear:
	$(call env_remove,$(LIBPTHREAD_O),)

	$(call env_remove,$(_LIBPTHREAD_A_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(LIBPTHREAD_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(LIBPTHREAD_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
