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

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

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

$(LIBDATA_LO):		$(LIBDATA_O)
	$(call remove,$(LIBDATA_LO),)

	$(call archive,$(LIBDATA_LO),$(LIBDATA_O),)

clear:
	$(call remove,$(LIBDATA_O),)

	$(call remove,$(LIBDATA_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(LIBDATA_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(LIBDATA_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
