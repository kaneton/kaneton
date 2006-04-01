##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/id/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       julien quintard   [sat apr  1 23:40:00 2006]
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

ID_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/id.h

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
