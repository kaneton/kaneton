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

include			../../env/.env.mk

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
	$(call static-library,$(_CRT_A_),$(CRT_O),)

clear:
	$(call remove,$(CRT_O),)

	$(call remove,$(_CRT_A_),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CRT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
