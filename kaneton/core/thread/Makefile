#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/thread/Makefile
#
# created       julien quintard   [sun jun 10 17:10:24 2007]
# updated       julien quintard   [sun jun 10 17:11:17 2007]
#

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

THREAD_C		:=		thread.c

THREAD_O		:=		$(THREAD_C:.c=.o)

THREAD_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/thread.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_THREAD_LO_)

$(_THREAD_LO_):		$(THREAD_O)
	$(call env_remove,$(_THREAD_LO_),)

	$(call env_archive,$(_THREAD_LO_),$(THREAD_O),)

clear:
	$(call env_remove,$(THREAD_O),)

	$(call env_remove,$(_THREAD_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(THREAD_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(THREAD_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
