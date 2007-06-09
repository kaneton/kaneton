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

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

CRT_C			:=		crt.c

CRT_O			:=		$(CRT_C:.c=.o)

CRT_INCLUDE		:=

#
# ---------- rules ------------------------------------------------------------
#

$(_CRT_A_):		$(CRT_O)
	$(call env_static-library,$(_CRT_A_),$(CRT_O),)

clear:
	$(call env_remove,$(CRT_O),)

	$(call env_remove,$(_CRT_A_),)

	$(call env_purge,)

proto:

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(CRT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
