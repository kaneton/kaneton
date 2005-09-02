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
## last update   Wed Aug 31 16:16:34 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../.kaneton.mk

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

LIBSYS_INCLUDE		:=		$(_LIBS_DIR_)/klibc/include/libsys/libsys.h

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

include			./$(_MAKEFILE_MK_)
