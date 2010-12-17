#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ne/glue/ibm-pc.ia32/educational/Makefile
#
# created       julien quintard   [sun jun 10 15:47:23 2007]
# updated       julien quintard   [fri dec 17 20:33:46 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/machine/glue

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

GLUE_C			:=		as.c				\
					clock.c				\
					cpu.c				\
					event.c				\
					io.c				\
					kernel.c			\
					map.c				\
					message.c			\
					region.c			\
					scheduler.c			\
					segment.c			\
					task.c				\
					thread.c			\
					timer.c				

GLUE_O			:=		$(GLUE_C:.c=.o)

GLUE_INCLUDE		:=		$(_GLUE_INCLUDE_DIR_)/as.h	\
					$(_GLUE_INCLUDE_DIR_)/clock.h	\
					$(_GLUE_INCLUDE_DIR_)/cpu.h	\
					$(_GLUE_INCLUDE_DIR_)/event.h	\
					$(_GLUE_INCLUDE_DIR_)/io.h	\
					$(_GLUE_INCLUDE_DIR_)/kernel.h	\
					$(_GLUE_INCLUDE_DIR_)/map.h	\
					$(_GLUE_INCLUDE_DIR_)/message.h	\
					$(_GLUE_INCLUDE_DIR_)/region.h	\
					$(_GLUE_INCLUDE_DIR_)/scheduler.h \
					$(_GLUE_INCLUDE_DIR_)/segment.h	\
					$(_GLUE_INCLUDE_DIR_)/task.h	\
					$(_GLUE_INCLUDE_DIR_)/thread.h	\
					$(_GLUE_INCLUDE_DIR_)/timer.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_GLUE_LO_)

$(_GLUE_LO_):		$(GLUE_O)
	$(call env_remove,$(_GLUE_LO_),)

	$(call env_archive,$(_GLUE_LO_),$(GLUE_O),)

clear:
	$(call env_remove,$(GLUE_O),)

	$(call env_remove,$(_GLUE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(GLUE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(GLUE_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
