#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/library/libc/libdata/Makefile
#
# created       julien quintard   [thu jun 28 20:47:38 2007]
# updated       matthieu bucchianeri   [mon aug  6 17:12:33 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		libc/libdata

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

LIBDATA_LO		:=		libdata.lo

LIBDATA_C		:=		dump.c				\
					alloc.c				\
					sum2.c

LIBDATA_O		:=		$(LIBDATA_C:.c=.o)

LIBDATA_INCLUDE		:=		$(_ULIBC_INCLUDE_DIR_)/libdata/libdata.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(LIBDATA_LO)

$(LIBDATA_LO):		$(LIBDATA_O)
	$(call env_remove,$(LIBDATA_LO),)

	$(call env_archive,$(LIBDATA_LO),$(LIBDATA_O),)

clear:
	$(call env_remove,$(LIBDATA_O),)

	$(call env_remove,$(LIBDATA_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(LIBDATA_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(LIBDATA_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
