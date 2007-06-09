##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/segment/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       julien quintard   [sat apr  1 23:39:29 2006]
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

SEGMENT_LO		:=		segment.lo

SEGMENT_C		:=		segment.c		\
					$(_SEGMENT_FILE_)

SEGMENT_O		:=		$(SEGMENT_C:.c=.o)

SEGMENT_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/segment.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(SEGMENT_LO):		$(SEGMENT_O)
	$(call env_remove,$(SEGMENT_LO),)

	$(call env_archive,$(SEGMENT_LO),$(SEGMENT_O),)

clear:
	$(call env_remove,$(SEGMENT_O),)

	$(call env_remove,$(SEGMENT_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(SEGMENT_INCLUDE),)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(SEGMENT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
