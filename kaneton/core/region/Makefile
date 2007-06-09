##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/region/Makefile
##
## created       julien quintard   [wed nov 23 09:19:13 2005]
## updated       julien quintard   [sat apr  1 23:39:45 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

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

REGION_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/region.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(REGION_LO):		$(REGION_O)
	$(call env_remove,$(REGION_LO),)

	$(call env_archive,$(REGION_LO),$(REGION_O),)

clear:
	$(call env_remove,$(REGION_O),)

	$(call env_remove,$(REGION_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(REGION_INCLUDE),)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(REGION_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
