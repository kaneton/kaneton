##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/kaneton/region/Makefile
##
## created       julien quintard   [wed nov 23 09:19:13 2005]
## updated       matthieu bucchianeri   [tue jan 10 01:27:17 2006]
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

REGION_LO		:=		region.lo

REGION_C		:=		region.c		\
					$(_REGION_FILE_)

REGION_O		:=		$(REGION_C:.c=.o)

REGION_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/region.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(REGION_LO):		$(REGION_O)
	$(call remove,$(REGION_LO),)

	$(call archive,$(REGION_LO),$(REGION_O),)

clear:
	$(call remove,$(REGION_O),)

	$(call remove,$(REGION_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(REGION_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(REGION_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
