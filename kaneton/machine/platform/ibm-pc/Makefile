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
# updated       julien quintard   [thu dec  9 14:37:23 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/machine/platform

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

PLATFORM_C		:=	console.c				\
				pic.c					\
				pit.c					\
				rtc.c					\
				serial.c				

PLATFORM_O		:=	$(PLATFORM_C:.c=.o)

PLATFORM_INCLUDE	:=	$(_PLATFORM_INCLUDE_DIR_)/console.h	\
				$(_PLATFORM_INCLUDE_DIR_)/pic.h		\
				$(_PLATFORM_INCLUDE_DIR_)/pit.h		\
				$(_PLATFORM_INCLUDE_DIR_)/rtc.h		\
				$(_PLATFORM_INCLUDE_DIR_)/serial.h	

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
