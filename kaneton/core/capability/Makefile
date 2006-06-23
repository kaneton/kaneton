##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/capability/Makefile
##
## created       julien quintard   [sat dec 10 13:50:17 2005]
## updated       julien quintard   [fri jun 23 21:12:33 2006]
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

CAPABILITY_LO		:=		capability.lo

CAPABILITY_C		:=		capability.c			

CAPABILITY_O		:=		$(CAPABILITY_C:.c=.o)

CAPABILITY_INCLUDE	:=		$(_CORE_INCLUDE_DIR_)/capability.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(CAPABILITY_LO):		$(CAPABILITY_O)
	$(call remove,$(CAPABILITY_LO),)

	$(call archive,$(CAPABILITY_LO),$(CAPABILITY_O),)

clear:
	$(call remove,$(CAPABILITY_O),)

	$(call remove,$(CAPABILITY_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(CAPABILITY_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CAPABILITY_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
