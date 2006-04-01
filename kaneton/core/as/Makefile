##
## licence       kaneton licence
##
## project       knaeton
##
## file          /home/buckman/kaneton/core/kaneton/as/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:39:52 2006]
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

AS_LO			:=		as.lo

AS_C			:=		as.c

AS_O			:=		$(AS_C:.c=.o)

AS_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/as.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(AS_LO):		$(AS_O)
	$(call remove,$(AS_LO),)

	$(call archive,$(AS_LO),$(AS_O),)

clear:
	$(call remove,$(AS_O),)

	$(call remove,$(AS_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(AS_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(AS_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
