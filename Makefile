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
## last update   Mon Oct 10 18:17:56 2005   mycure
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
	  echo "   ----- you can now run your command..."		; \
	  echo ""							; \
	fi

init:
	cd env								; \
	$(SHELL) init							; \
	cd ..

clean:			kaneton.mk
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

view-%:			kaneton.mk
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

info:			kaneton.mk
	$(call print,white,,)

	$(call print,blue,--- ,-n)
	$(call print,white,kaneton,)

	$(call print,white,,)

	$(call print,blue,# ,-n)
	$(call print,white,authors:,)
	$(call print,white,,)
	$(call print,white,     julien quintard - quinta_j@epita.fr,)
	$(call print,white,     jean-pascal billaud - billau_j@epita.fr,)

	$(call print,white,,)

	$(call print,blue,# ,-n)
	$(call print,white,assistants:,)
	$(call print,white,,)
	$(call print,white,     cedric aubouy - aubouy_c@epita.fr,)
	$(call print,white,     fabien le-mentec - le-men_f@epita.fr,)
	$(call print,white,     renaud lienhart - lienha_r@epita.fr,)

	$(call print,white,,)

	$(call print,blue,# ,-n)
	$(call print,white,contributors:,)
	$(call print,white,,)
	$(call print,white,     christophe eymard - eymard_c@epita.fr,)
	$(call print,white,     solal jacob - jacob_s@epitech.net,)

	$(call print,white,,)

	$(call print,blue,# ,-n)
	$(call print,white,website:,)
	$(call print,white,,)
	$(call print,white,     http://www.lse.epita.fr/,)

	$(call print,white,,)

	$(call print,blue,# ,-n)
	$(call print,white,news-group:,)
	$(call print,white,,)
	$(call print,white,     epita.cours.kaneton,)

	$(call print,white,,)

	$(call print,blue,# ,-n)
	$(call print,white,mailing-list:,)
	$(call print,white,,)
	$(call print,white,     kaneton@lse.epita.fr,)

	$(call print,white,,)

#
# ---------- distribution -----------------------------------------------------
#

dist:			kaneton.mk kaneton
	$(call dist)
