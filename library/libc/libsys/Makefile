#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/library/libc/libsys/Makefile
#
# created       julien quintard   [thu jun 28 20:48:42 2007]
# updated       matthieu bucchianeri   [mon aug  6 17:13:18 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		libc/libsys

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

LIBSYS_LO		:=		libsys.lo

LIBSYS_C		:=		perror.c

LIBSYS_O		:=		$(LIBSYS_C:.c=.o)

LIBSYS_INCLUDE		:=		$(_LIBC_INCLUDE_DIR_)/libsys/libsys.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(LIBSYS_LO)

$(LIBSYS_LO):		$(LIBSYS_O)
	$(call env_remove,$(LIBSYS_LO),)

	$(call env_archive,$(LIBSYS_LO),$(LIBSYS_O),)

clear:
	$(call env_remove,$(LIBSYS_O),)

	$(call env_remove,$(LIBSYS_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(LIBSYS_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(LIBSYS_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
