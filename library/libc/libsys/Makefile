##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/klibc/libsys/Makefile
##
## created       julien quintard   [fri feb 11 03:05:40 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:59:39 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

LIBSYS_LO		:=		libsys.lo

LIBSYS_C		:=		perror.c			\
					quad.c

LIBSYS_O		:=		$(LIBSYS_C:.c=.o)

LIBSYS_INCLUDE		:=		$(_KLIBC_INCLUDE_DIR_)/libsys/libsys.h

#
# ---------- rules ------------------------------------------------------------
#

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

	$(call env_headers,$(LIBSYS_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
