#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...kaneton/machine/platform/ibm-pc/Makefile
#
# created       julien quintard   [sun dec 10 18:38:17 2006]
# updated       julien quintard   [thu jun 28 20:04:58 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		platform

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

PLATFORM_C		:=		cons-text.c			\
					irq.c				\
					keyboard.c			\
					serial.c			\
					timer.c

PLATFORM_O		:=		$(PLATFORM_C:.c=.o)

PLATFORM_INCLUDE	:=	$(_PLATFORM_INCLUDE_DIR_)/cons.h	\
				$(_PLATFORM_INCLUDE_DIR_)/irq.h		\
				$(_PLATFORM_INCLUDE_DIR_)/keyboard.h	\
				$(_PLATFORM_INCLUDE_DIR_)/serial.h	\
				$(_PLATFORM_INCLUDE_DIR_)/timer.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_PLATFORM_LO_)

$(_PLATFORM_LO_):		$(PLATFORM_O)
	$(call env_remove,$(_PLATFORM_LO_),)

	$(call env_archive,$(_PLATFORM_LO_),$(PLATFORM_O),)

clear:
	$(call env_remove,$(PLATFORM_O),)

	$(call env_remove,$(_PLATFORM_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(PLATFORM_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(PLATFORM_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
