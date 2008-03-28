#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/lec_l/kaneton/kaneton/machine/platform/octane/Makefile
#
# created       laurent lec   [fri mar 28 17:13:50 2008]
# updated       laurent lec   [fri mar 28 17:13:52 2008]
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

PLATFORM_C		:=	cons.c

PLATFORM_O		:=	$(PLATFORM_C:.c=.o)

PLATFORM_INCLUDE	:=	$(_PLATFORM_INCLUDE_DIR_)/cons.h

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
