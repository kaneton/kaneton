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

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

ID_C			:=		id.c

ID_O			:=		$(ID_C:.c=.o)

ID_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/id.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			dependencies $(_ID_LO_)

$(_ID_LO_):		$(ID_O) $(_LIBC_A_)
	$(call env_remove,$(_ID_LO_),)

	$(call env_archive,$(_ID_LO_),$(ID_O),)

clear:
	$(call env_remove,$(ID_O),)

	$(call env_remove,$(_ID_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(ID_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(ID_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
