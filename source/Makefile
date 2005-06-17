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
## started on    Fri Feb 11 02:04:24 2005   mycure
## last update   Fri Jun 17 15:50:29 2005   mycure
##

all:			kaneton

#
# dependency
#

.kaneton.mk:
	@if [ ! -e .kaneton.mk ] ; then					\
	make init							; \
	fi

#
# environment
#

init:
	@cd env								; \
	./init								; \
	cd ..

clean:
	@cd env								; \
	./clean								; \
	cd ..

#
# development
#

SUBDIRS			=	libs core services

kaneton:		.kaneton.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make								; \
	cd ..								; \
	done

distclean:		.kaneton.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make distclean							; \
	cd ..								; \
	done

	@rm -f *~ .*~ \#~ .\#~

proto:			.kaneton.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make proto							; \
	cd ..								; \
	done

#
# distribution
#

CONF			=		../conf/$(USER)/$(USER).conf

BOOTLOADER		=		`cat $(CONF)			| \
					 grep -E "^BOOTLOADER = .*$$"	| \
					 cut -b 14-`

boot-init:		.kaneton.mk
	@cd utils							; \
	./$(BOOTLOADER) init						; \
	cd ..

boot-install:		.kaneton.mk
	@cd utils/							; \
	./$(BOOTLOADER) install						; \
	cd ..
