##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/interface/Makefile
##
## created       julien quintard   [sat dec 10 13:50:17 2005]
## updated       julien quintard   [fri jun 23 21:17:52 2006]
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

INTERFACE_LO		:=		interface.lo

INTERFACE_C		:=		interface.c			

INTERFACE_O		:=		$(INTERFACE_C:.c=.o)

INTERFACE_INCLUDE	:=		$(_CORE_INCLUDE_DIR_)/interface.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(INTERFACE_LO):		$(INTERFACE_O)
	$(call remove,$(INTERFACE_LO),)

	$(call archive,$(INTERFACE_LO),$(INTERFACE_O),)

clear:
	$(call remove,$(INTERFACE_O),)

	$(call remove,$(INTERFACE_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(INTERFACE_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(INTERFACE_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
