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
## last update   Sun Jun 12 22:11:14 2005   mycure
##

include			../.kaneton.mk

SUBDIRS			=		bootstrap bootloader kaneton

all:
	@for i in $(SUBDIRS) ; do					\
	$(CD) $$i							; \
	$(MAKE)								; \
	$(CD) ..							; \
	done

distclean:
	@for i in $(SUBDIRS) ; do					\
	$(CD) $$i							; \
	$(MAKE) distclean						; \
	$(CD) ..							; \
	done

	$(call purge,)

proto:
	@for i in $(SUBDIRS) ; do					\
	$(CD) $$i							; \
	$(MAKE) proto							; \
	$(CD) ..							; \
	done
