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
## last update   Sun Jun 12 21:14:28 2005   mycure
##

include			../.kaneton.mk

SUBDIRS			=		bootstrap bootloader kaneton

all:
	@for i in $(SUBDIRS) ; do					\
	cd $$i								; \
	make								; \
	cd ..								; \
	done

distclean:
	@for i in $(SUBDIRS) ; do					\
	cd $$i								; \
	make distclean							; \
	cd ..								; \
	done

	@$(_PURGE_)

proto:
	@for i in $(SUBDIRS) ; do					\
	cd $$i								; \
	make proto							; \
	cd ..								; \
	done
