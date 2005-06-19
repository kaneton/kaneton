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
## last update   Sun Jun 19 20:52:10 2005   mycure
##

#
# ---------- directives -------------------------------------------------------
#

.SILENT:

.PHONY:		all init clean kaneton clear purge proto		\
		dep build install check

#
# ---------- shell ------------------------------------------------------------
#

SHELL			:=		/bin/sh

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			=	libs core services

CONF_BOOTLOADER		:=						\
  $(shell cat conf/$(USER)/$(USER).conf					| \
          grep -E "^BOOTLOADER = .*$$"					| \
          cut -b 14-)

#
# ---------- default rule -----------------------------------------------------
#

all:			kaneton

#
# ---------- environment dependency -------------------------------------------
#

.kaneton.mk:
	@if [ ! -e .kaneton.mk ] ; then					\
	make init							; \
	fi

#
# ---------- environment ------------------------------------------------------
#

init:
	@cd env								; \
	./init								; \
	cd ..

clean:			purge
	@cd env								; \
	./clean								; \
	cd ..

#
# ---------- development ------------------------------------------------------
#

kaneton:		.kaneton.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make								; \
	cd ..								; \
	done

clear:			.kaneton.mk purge
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make clear							; \
	cd ..								; \
	done

purge:
	@rm -f *~ .*~ \#~ .\#~

#
# ---------- prototypes -------------------------------------------------------
#

proto:			.kaneton.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make proto							; \
	cd ..								; \
	done

#
# ---------- dependencies -----------------------------------------------------
#

dep:			.kaneton.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make dep							; \
	cd ..								; \
	done

#
# ---------- distribution -----------------------------------------------------
#

build:			.kaneton.mk
	@cd utils							; \
	./$(CONF_BOOTLOADER) build					; \
	cd ..

install:		.kaneton.mk
	@cd utils/							; \
	./$(CONF_BOOTLOADER) install					; \
	cd ..

#
# ---------- check ------------------------------------------------------------
#

check:			.kaneton.mk

#
# ---------- info -------------------------------------------------------------
#

info:
	@echo ""
	@echo -e '\E[;34m'"\033[1m---\033[0m \033[1mkaneton\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1mcreators:\033[0m"
	@echo ""
	@echo -e "     \033[1mquintard julien - quinta_j@epita.fr\033[0m"
	@echo ""
	@echo -e "     \033[1mbillaud jean-pascal - billau_j@epita.fr\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1mcontributors:\033[0m"
	@echo ""
	@echo -e "     \033[1mrenaud lienhart - lienha_r@epita.fr\033[0m"
	@echo ""
