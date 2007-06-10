##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/klibc/libdata/Makefile
##
## created       julien quintard   [fri feb 11 03:05:40 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:54:42 2006]
##

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

LIBDATA_INCLUDE		:=		$(_KLIBC_INCLUDE_DIR_)/libdata/libdata.h

#
# ---------- rules ------------------------------------------------------------
#

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

	$(call env_headers,$(LIBDATA_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
