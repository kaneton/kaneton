#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...e/architecture/ia32/educational/Makefile
#
# created       julien quintard   [sun dec 10 18:38:17 2006]
# updated       julien quintard   [sun jun 10 21:21:28 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

ARCHITECTURE_C		:=		io.c				\
					as.c				\
					region.c			\
					segment.c			\
					event.c				\
					timer.c				\
					thread.c			\
					task.c				\
					sched.c				\
					message.c			\
					cpu.c				

ARCHITECTURE_O		:=		$(ARCHITECTURE_C:.c=.o)

ARCHITECTURE_INCLUDE	:=		$(_ARCHITECTURE_INCLUDE_DIR_)/core.h

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_ARCHITECTURE_LO_)

$(_ARCHITECTURE_LO_):		$(ARCHITECTURE_O)
	$(call env_remove,$(_ARCHITECTURE_LO_),)

	$(call env_archive,$(_ARCHITECTURE_LO_),$(ARCHITECTURE_O),)

clear:
	$(call env_remove,$(ARCHITECTURE_O),)

	$(call env_remove,$(_ARCHITECTURE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(ARCHITECTURE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(ARCHITECTURE_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
