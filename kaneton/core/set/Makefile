#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/set/Makefile
#
# created       julien quintard   [sun jun 10 17:04:39 2007]
# updated       julien quintard   [tue jun 26 15:52:08 2007]
#

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

main:			dependencies $(_SET_LO_)

$(_SET_LO_):		$(SET_O) $(_LIBC_A_)
	$(call env_remove,$(_SET_LO_),)

	$(call env_archive,$(_SET_LO_),$(SET_O),)

clear:
	$(call env_remove,$(SET_O),)

	$(call env_remove,$(_SET_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(SET_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(SET_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
