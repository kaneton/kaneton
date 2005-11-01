##
## copyright quintard julien
##
## kaneton
##
## Makefile
##
## path          /home/ultima/g/kaneton/env
##
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
##
## started on    Fri Feb 11 02:04:24 2005   mycure
## last update   Tue Nov  1 17:06:58 2005   cedric
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
		dep build install check info 				\
		view view- export export-

#
# ---------- variables --------------------------------------------------------
#

SHELL			:=		/bin/sh

#
# ---------- default rule -----------------------------------------------------
#

ifeq ($(_SIGNATURE_),kaneton)
  all:			kaneton
else
  all kaneton								\
  clear purge								\
  proto dep								\
  build install								\
  check info								\
  view view-								\
  export export-							\
  view-% export-%:	init
	$(MAKE) -f Makefile $@
endif

#
# ---------- environment ------------------------------------------------------
#

init:
	cd env								; \
	$(SHELL) init.sh						; \
	cd ..

clean:
	cd env								; \
	$(SHELL) clean.sh						; \
	cd ..

#
# ---------- conditional ------------------------------------------------------
#

ifeq ($(_SIGNATURE_),kaneton)

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=						\
  $(sort								\
      $(shell $(_CAT_) $(_CONF_DIR_)/$(USER)/modules.conf |		\
              $(_SED_) -n "s/^\([[:alpha:]_-]*\)\/.*$$/\1/p"))

CLEARDIRS		:=		libs view export

#
# ---------- development ------------------------------------------------------
#

kaneton:
	$(call make,$(SUBDIRS),)

clear:
	$(call make,$(SUBDIRS),clear)

	$(call make,$(CLEARDIRS),clear)

purge:
	$(call purge,)

#
# ---------- prototypes -------------------------------------------------------
#

proto:
	$(call make,$(SUBDIRS),proto)

#
# ---------- dependencies -----------------------------------------------------
#

dep:
	$(call make,$(SUBDIRS),dep)

#
# ---------- boot -------------------------------------------------------------
#

build:
	cd $(_MULTIBOOTLOADERS_DIR_)/$(_MULTIBOOTLOADER_)		; \
	$(SHELL) $(_MULTIBOOTLOADER_).sh build				; \
	cd ..

install:		kaneton
	cd $(_MULTIBOOTLOADERS_DIR_)/$(_MULTIBOOTLOADER_)		; \
	$(SHELL) $(_MULTIBOOTLOADER_).sh install			; \
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

view- view:
	cd $(_VIEW_DIR_)						; \
	$(SHELL) $(_VIEW_SH_)						; \
	cd ..

view-%:
	cd $(_VIEW_DIR_) 						; \
	$(SHELL) $(_VIEW_SH_) $*					; \
	cd ..

#
# ---------- information ------------------------------------------------------
#

info:
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
	$(call print,white,     renaud lienhart - lienha_r@epita.fr,)

	$(call print,white,,)

	$(call print,blue,# ,-n)
	$(call print,white,contributors:,)
	$(call print,white,,)
	$(call print,white,     christophe eymard - eymard_c@epita.fr,)
	$(call print,white,     solal jacob - jacob_s@epitech.net,)
	$(call print,white,     fabien le-mentec - le-men_f@epita.fr,)

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
# ---------- dist -------------------------------------------------------------
#

dist:			export-dist

#
# ---------- export -----------------------------------------------------------
#

export- export:
	cd $(_EXPORT_DIR_)						; \
	$(SHELL) $(_EXPORT_SH_)						; \
	cd ..

export-%:
	cd $(_EXPORT_DIR_)						; \
	$(SHELL) $(_EXPORT_SH_)						; \
	cd ..

#
# ---------- /conditional -----------------------------------------------------
#

endif
