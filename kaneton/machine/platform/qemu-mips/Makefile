#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/enguerrand/...eton/machine/platform/qemu-mips/Makefile
#
# created       enguerrand raymond   [sat oct 10 00:19:17 2008]
# updated          [wed nov 26 01:24:21 2008]
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

PLATFORM_C		:=		timer.c

PLATFORM_O		:=		$(PLATFORM_C:.c=.o)

PLATFORM_INCLUDE	:=

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
