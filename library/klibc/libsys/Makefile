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

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

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
	$(call remove,$(LIBSYS_LO),)

	$(call archive,$(LIBSYS_LO),$(LIBSYS_O),)

clear:
	$(call remove,$(LIBSYS_O),)

	$(call remove,$(LIBSYS_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(LIBSYS_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(LIBSYS_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
