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
## started on    Fri Feb 11 02:04:24 2005   mycure
## last update   Sat Jun 18 11:58:05 2005   mycure
##

#
# directives
#

.SILENT:

.PHONY:		all init clean kaneton clear proto dep build install check

#
# shell
#

SHELL			:=		/bin/sh

#
# the default rule
#

all:			kaneton

#
# environment dependency
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

clear:			.kaneton.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make clear							; \
	cd ..								; \
	done

	@rm -f *~ .*~ \#~ .\#~

#
# prototypes
#

proto:			.kaneton.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make proto							; \
	cd ..								; \
	done

#
# dependencies
#

dep:			.kaneton.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make dep							; \
	cd ..								; \
	done

#
# distribution
#

build:			.kaneton.mk
	@cd utils							; \
	./$(_CONF_BOOTLOADER_) build					; \
	cd ..

install:		.kaneton.mk
	@cd utils/							; \
	./$(_CONF_BOOTLOADER_) install					; \
	cd ..

#
# check
#

check:			.kaneton.mk
	;
