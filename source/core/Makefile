##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:24:11 2005   mycure
## last update   Sun Jun 12 22:54:54 2005   mycure
##

include			../.kaneton.mk

SUBDIRS			=		bootstrap bootloader kaneton

all:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_)							; \
	$(_CD_) ..							; \
	done

distclean:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_) distclean						; \
	$(_CD_) ..							; \
	done

	$(call purge,)

proto:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_) proto							; \
	$(_CD_) ..							; \
	done
