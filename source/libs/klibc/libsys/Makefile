##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/libs/klibc/libsys
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:05:40 2005   mycure
## last update   Thu May  5 23:03:20 2005   mycure
##

include			../../../.common.mk

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
