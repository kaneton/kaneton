#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/library/crt/Makefile
#
# created       julien quintard   [thu jun 28 19:52:18 2007]
# updated       julien quintard   [thu jun 28 19:52:50 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		crt

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

CRT_C			:=		crt.c

CRT_O			:=		$(CRT_C:.c=.o)

CRT_INCLUDE		:=

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_CRT_A_)

$(_CRT_A_):		$(CRT_O)
	$(call env_static-library,$(_CRT_A_),$(CRT_O),)

clear:
	$(call env_remove,$(CRT_O),)

	$(call env_remove,$(_CRT_A_),)

	$(call env_purge,)

prototypes:

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(CRT_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
