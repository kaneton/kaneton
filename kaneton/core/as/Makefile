##
## licence       kaneton licence
##
## project       knaeton
##
## file          /home/mycure/kaneton/kaneton/core/as/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       julien quintard   [sat apr  1 23:40:38 2006]
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

AS_LO			:=		as.lo

AS_C			:=		as.c

AS_O			:=		$(AS_C:.c=.o)

AS_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/as.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(AS_LO):		$(AS_O)
	$(call env_remove,$(AS_LO),)

	$(call env_archive,$(AS_LO),$(AS_O),)

clear:
	$(call env_remove,$(AS_O),)

	$(call env_remove,$(AS_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(AS_INCLUDE),)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(AS_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
