#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/kaneton/core/thread/Makefile
#
# created       julien quintard   [sun jun 10 17:10:24 2007]
# updated       matthieu bucchianeri   [sat sep  1 12:29:27 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/thread

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

THREAD_C		:=		thread.c

THREAD_O		:=		$(THREAD_C:.c=.o)

THREAD_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/thread.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_THREAD_LO_)

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

	$(call env_headers,$(THREAD_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
