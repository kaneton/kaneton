##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/core/kaneton/id/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:38:37 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

ID_LO			:=		id.lo

ID_C			:=		id.c

ID_O			:=		$(ID_C:.c=.o)

ID_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/id.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(ID_LO):		$(ID_O)
	$(call remove,$(ID_LO),)

	$(call archive,$(ID_LO),$(ID_O),)

clear:
	$(call remove,$(ID_O),)

	$(call remove,$(ID_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(ID_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(ID_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
