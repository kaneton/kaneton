##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/libs/klibc/libsys
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:05:40 2005   mycure
## last update   Mon Nov  7 15:50:38 2005   mycure
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
