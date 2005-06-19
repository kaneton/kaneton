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
## started on    Fri Feb 11 02:29:19 2005   mycure
## last update   Sun Jun 19 21:35:49 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		libc libsys

LIB_C			:=		klibc.a
LIB_H			:=		include/klibc.h

LIBC			:=		libc/libc.a
LIBSYS			:=		libsys/libsys.a

#
# ---------- rules ------------------------------------------------------------
#

$(LIB_C):		libs
	$(call archives,$(LIB_C),$(LIBC) $(LIBSYS),)

	$(call link,$(_LIBC_A_),klibc/$(LIB_C),)
	$(call link,$(_LIBC_H_),klibc/$(LIB_H),)

libs:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_) 							; \
	$(_CD_) ..							; \
	done

clear:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i 							; \
	$(_MAKE_) clear 						; \
	$(_CD_) .. 							; \
	done

	$(call remove,$(LIB_C),)

	$(call remove,$(_LIBC_A_),)
	$(call remove,$(_LIBC_H_),)

	$(call purge,)

proto:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i 							; \
	$(_MAKE_) proto 						; \
	$(_CD_) .. 							; \
	done

dep:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i 							; \
	$(_MAKE_) dep 							; \
	$(_CD_) .. 							; \
	done
