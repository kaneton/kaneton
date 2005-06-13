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
## last update   Sun Jun 12 22:41:25 2005   mycure
##

include			../../../.kaneton.mk

LIBNAME			=		libsys.a
LIBINCLUDE		=		../include/libsys/libsys.h

LIBSYS_O		=		perror.o			


all:			$(LIBSYS_O)
	$(call archive,$(LIBNAME),$(LIBSYS_O),)

	$(call static-library,$(LIBNAME),)

distclean:
	$(call remove,$(LIBSYS_O),)

	$(call purge,)

	$(call remove,$(LIBNAME),)

proto:
	$(call prototypes,$(LIBINCLUDE),)
