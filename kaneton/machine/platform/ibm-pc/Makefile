#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kan...kaneton/machine/platform/ibm-pc/Makefile
#
# created       julien quintard   [sun dec 10 18:38:17 2006]
# updated       matthieu bucchianeri   [sun jun 17 17:35:41 2007]
#

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

PLATFORM_C		:=	cons-text.c		\
				event.c			\
				serial.c		\
				timer.c

PLATFORM_O		:=	$(PLATFORM_C:.c=.o)

PLATFORM_INCLUDE	:=	$(_PLATFORM_INCLUDE_DIR_)/cons.h	\
				$(_PLATFORM_INCLUDE_DIR_)/event.h	\
				$(_PLATFORM_INCLUDE_DIR_)/serial.h	\
				$(_PLATFORM_INCLUDE_DIR_)/timer.h

#
# ---------- rules ------------------------------------------------------------
#

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

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
