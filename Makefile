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
## last update   Mon Sep 26 05:16:08 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

-include		./.kaneton.mk

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

_SHELL_			?=		true

SUBDIRS			:=		libs core services

#
# ---------- default rule -----------------------------------------------------
#

all:			kaneton

#
# ---------- environment ------------------------------------------------------
#

kaneton.mk:
	if [ ! -e .kaneton.mk ] ; then					\
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
	cd env								; \
	$(SHELL) init							; \
	cd ..

clean:
	cd env								; \
	$(SHELL) clean							; \
	cd ..

	rm -f *~

#
# ---------- development ------------------------------------------------------
#

kaneton:		kaneton.mk
	$(call make,$(SUBDIRS),)

clear:			kaneton.mk
	$(call make,$(SUBDIRS),clear)

	$(call make,papers/,clear)

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
	cd utils/							; \
	$(_SHELL_) $(_MULTIBOOTLOADER_) build				; \
	cd ..

install:		kaneton.mk kaneton
	cd utils/							; \
	$(_SHELL_) $(_MULTIBOOTLOADER_) install				; \
	cd ..

#
# ---------- check ------------------------------------------------------------
#
# XXX
#

check:

#
# ---------- view -------------------------------------------------------------
#

view-%:
	if [ $* == ".o" ] ; then					\
	  ARG=""							; \
	else								\
	  ARG=$*							; \
	fi								; \
									\
	cd papers/							; \
	$(_SHELL_) $(_VIEWER_) $$ARG					; \
	cd ..								; \
									\
	if [ $* == ".o" ] ; then					\
	  exit 2							; \
	fi

#
# ---------- information ------------------------------------------------------
#

info:
	echo ""
	echo -e '\E[;34m'"\033[1m---\033[0m \033[1mkaneton\033[0m"
	echo ""
	echo -e '\E[;34m'"\033[1m#\033[0m \033[1mauthors:\033[0m"
	echo ""
	echo -e "     \033[1mquintard julien - quinta_j@epita.fr\033[0m"
	echo -e "     \033[1mbillaud jean-pascal - billau_j@epita.fr\033[0m"
	echo ""
	echo -e '\E[;34m'"\033[1m#\033[0m \033[1massistants:\033[0m"
	echo ""
	echo -e "     \033[1mcedric aubouy - aubouy_c@epita.fr\033[0m"
	echo -e "     \033[1mfabien le-mentec - le-men_f@epita.fr\033[0m"
	echo -e "     \033[1mrenaud lienhart - lienha_r@epita.fr\033[0m"
	echo ""
	echo -e '\E[;34m'"\033[1m#\033[0m \033[1mcontributors:\033[0m"
	echo ""
	echo -e "     \033[1mchristophe eymard - eymard_c@epita.fr\033[0m"
	echo -e "     \033[1msolal jacob - jacob_s@epitech.net\033[0m"
	echo ""
	echo -e '\E[;34m'"\033[1m#\033[0m \033[1mwebsite:\033[0m"
	echo ""
	echo -e "     \033[1mhttp://www.lse.epita.fr/\033[0m"
	echo ""
	echo -e '\E[;34m'"\033[1m#\033[0m \033[1mnews-group:\033[0m"
	echo ""
	echo -e "     \033[1mepita.cours.kaneton\033[0m"
	echo ""
	echo -e '\E[;34m'"\033[1m#\033[0m \033[1mmailing-list:\033[0m"
	echo ""
	echo -e "     \033[1mkaneton@lse.epita.fr\033[0m"
	echo ""

#
# ---------- distribution -----------------------------------------------------
#

dist:			kaneton.mk kaneton
	$(call dist)
