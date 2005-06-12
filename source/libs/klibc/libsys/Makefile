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
## last update   Sun Jun 12 21:20:13 2005   mycure
##

include			../../../.kaneton.mk

LIBNAME			=		libsys.a
LIBINCLUDE		=		../include/libsys/libsys.h

LIBSYS_O		=		perror.o			


all:			$(LIBSYS_O)
	$(call ar-o-a,$(LIBNAME),$(LIBSYS_O),)
	$(call ranlib,$(LIBNAME),)

distclean:
	$(call rm,$(LIBSYS_O),)

	@$(_PURGE_)

	$(call rm,$(LIBNAME),)

proto:
	$(call proto,$(LIBINCLUDE),)
