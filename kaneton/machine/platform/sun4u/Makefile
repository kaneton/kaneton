#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kan.../kaneton/machine/platform/sun4u/Makefile
#
# created       julien quintard   [sun dec 10 18:38:17 2006]
# updated       matthieu bucchianeri   [sun jun 17 20:12:06 2007]
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

PLATFORM_C		:=

PLATFORM_O		:=	$(PLATFORM_C:.c=.o)

PLATFORM_INCLUDE	:=

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
