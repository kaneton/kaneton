##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/libs/klibc
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:05:40 2005   mycure
## last update   Mon Jul  4 12:58:08 2005   mycure
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

LIBSYS			:=		libsys.a

LIBSYS_C		:=		perror.c			\
					quad.c				

LIBSYS_O		:=		$(LIBSYS_C:.c=.o)

LIBSYS_INCLUDE		:=		$(_LIBS_DIR_)/klibc/include/libsys/libsys.h

#
# ---------- rules ------------------------------------------------------------
#

$(LIBSYS):		$(LIBSYS_O)
	$(call remove,$(LIBSYS),)

	$(call archive,$(LIBSYS),$(LIBSYS_O),)

	$(call static-library,$(LIBSYS),)

clear:
	$(call remove,$(LIBSYS_O),)

	$(call remove,$(LIBSYS),)

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
