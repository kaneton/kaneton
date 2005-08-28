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
## last update   Sun Aug 28 18:09:58 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			./.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.SILENT:

.PHONY:		all init clean kaneton clear purge proto		\
		dep build install check info dist kaneton.mk

#
# ---------- variables --------------------------------------------------------
#

SHELL			:=		/bin/sh

SUBDIRS			:=		libs core services

KANETON_MK_SIZE		:=		$(shell stat -c "%s" .kaneton.mk)

#
# ---------- default rule -----------------------------------------------------
#

all:			kaneton

#
# ---------- environment ------------------------------------------------------
#

kaneton.mk:
	@if [ $(KANETON_MK_SIZE) -eq 0 ] ; then				\
	  cd env							; \
	  $(SHELL) init							; \
	  cd ..								; \
									\
	  echo ""							; \
	  echo -n -e '\E[;31m'"\033[1m  -----\033[0m "			; \
	  echo -e "\033[1myou can now run your command...\033[0m"	; \
	  echo ""							; \
	fi

init:
	@cd env								; \
	$(SHELL) init							; \
	cd ..

clean:
	@cd env								; \
	$(SHELL) clean							; \
	cd ..

	@rm -f *~

#
# ---------- development ------------------------------------------------------
#

kaneton:		kaneton.mk
	$(call make,$(SUBDIRS),)

clear:			kaneton.mk
	$(call make,$(SUBDIRS),clear)

purge:			kaneton.mk
	$(call purge,)

#
# ---------- prototypes -------------------------------------------------------
#

proto:			kaneton.mk
	$(call make,$(SUBDIRS),proto)

#
# ---------- dependencies -----------------------------------------------------
#

dep:			kaneton.mk
	$(call make,$(SUBDIRS),dep)

#
# ---------- boot -------------------------------------------------------------
#

build:			kaneton.mk
	@cd utils							; \
	$(SHELL) $(_MULTIBOOTLOADER_) build				; \
	cd ..

install:		kaneton.mk kaneton
	@cd utils/							; \
	$(SHELL) $(_MULTIBOOTLOADER_) install				; \
	cd ..

#
# ---------- check ------------------------------------------------------------
#
# XXX
#

check:

#
# ---------- information ------------------------------------------------------
#

info:
	@echo ""
	@echo -e '\E[;34m'"\033[1m---\033[0m \033[1mkaneton\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1mauthors:\033[0m"
	@echo ""
	@echo -e "     \033[1mquintard julien - quinta_j@epita.fr\033[0m"
	@echo ""
	@echo -e "     \033[1mbillaud jean-pascal - billau_j@epita.fr\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1massistants:\033[0m"
	@echo ""
	@echo -e "     \033[1mcedric aubouy - aubouy_c@epita.fr\033[0m"
	@echo -e "     \033[1mfabien le-mentec - le-men_f@epita.fr\033[0m"
	@echo -e "     \033[1mrenaud lienhart - lienha_r@epita.fr\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1mcontributors:\033[0m"
	@echo ""
	@echo -e "     \033[1mchristophe eymard - eymard_c@epita.fr\033[0m"
	@echo -e "     \033[1mXXX - XXX@epitech.net\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1mnews-group:\033[0m"
	@echo ""
	@echo -e "     \033[1mepita.cours.kaneton\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1mmailing-list:\033[0m"
	@echo ""
	@echo -e "     \033[1mkaneton@lse.epita.fr\033[0m"
	@echo ""

#
# ---------- distribution -----------------------------------------------------
#

dist:			kaneton.mk kaneton
	$(call dist)
