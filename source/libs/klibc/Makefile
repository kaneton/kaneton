##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/libs/klibc
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:29:19 2005   mycure
## last update   Thu May  5 22:41:39 2005   mycure
##

include			../../.common.mk

SUBDIRS			=		libc libsys

LIBNAME			=		klibc.a
LIBINCLUDE		=		include/klibc.h

LIBC			=		libc/libc.a
LIBSYS			=		libsys/libsys.a

CFLAGS			+=		-D___kaneton

all:			sublibs
	$(call ar-a-a,$(LIBNAME),$(LIBC) $(LIBSYS),)

	@$(_LN_) klibc/$(LIBNAME) $(_LIBC_A_)
	@$(_LN_) klibc/$(LIBINCLUDE) $(_LIBC_H_)

sublibs:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_) 							; \
	$(_CD_) ..							; \
	done

distclean:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i 							; \
	$(_MAKE_) distclean 						; \
	$(_CD_) .. 							; \
	done

	@$(_PURGE_)

	$(call rm,$(LIBNAME),)

proto:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i 							; \
	$(_MAKE_) proto 						; \
	$(_CD_) .. 							; \
	done
