##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/services
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Sun Jun 12 22:41:55 2005   mycure
##

include			../.kaneton.mk

SUBDIRS			=		cons				\
					dsh				\
					kbd				\
					mod

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
