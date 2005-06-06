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
## started on    Fri Feb 11 03:24:11 2005   mycure
## last update   Sat Jun  4 22:19:21 2005   mycure
##

include			../.common.mk

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
