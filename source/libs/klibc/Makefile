##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:29:19 2005   mycure
## last update   Sun Jul  3 19:41:26 2005   mycure
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

SUBDIRS			:=		libdata libstring libsys

LIBC_A			:=		klibc.a
LIBC_H			:=		include/klibc.h

LIBDATA			:=		libdata/libdata.a
LIBSTRING		:=		libstring/libstring.a
LIBSYS			:=		libsys/libsys.a

#
# ---------- rules ------------------------------------------------------------
#

$(LIBC_A):		libs
	$(call archives,$(LIBC_A),$(LIBDATA) $(LIBSTRING) $(LIBSYS),)

	$(call link,$(_LIBC_A_),klibc/$(LIBC_A),)
	$(call link,$(_LIBC_H_),klibc/$(LIBC_H),)

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

	$(call remove,$(LIBC_A),)

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
