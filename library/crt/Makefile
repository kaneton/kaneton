##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/crt/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:47:55 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

CRT_C			:=		crt.c

CRT_O			:=		$(CRT_C:.c=.o)

CRT_INCLUDE		:=

#
# ---------- rules ------------------------------------------------------------
#

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

	$(call env_headers,$(CRT_C),$(_DEPENDENCY_MK_),)

dependencies:

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
