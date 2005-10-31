##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/export
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:04:24 2005   mycure
## last update   Mon Oct 31 11:55:22 2005   mycure
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
		view view- export export- kaneton.mk

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

kaneton.mk:
	if [ ! -e .kaneton.mk ] ; then					\
	  cd env							; \
	  $(SHELL) init.sh						; \
	  cd ..								; \
	fi

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

kaneton:		kaneton.mk
	$(call make,$(SUBDIRS),)

clear:			kaneton.mk
	$(call make,$(SUBDIRS),clear)

	$(call make,$(CLEARDIRS),clear)

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
	cd $(_MULTIBOOTLOADERS_DIR_)/$(_MULTIBOOTLOADER_)		; \
	$(SHELL) $(_MULTIBOOTLOADER_).sh build				; \
	cd ..

install:		kaneton.mk kaneton
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

view- view:		kaneton.mk
	cd $(_VIEW_DIR_)						; \
	$(SHELL) $(_VIEW_SH_)						; \
	cd ..

view-%:			kaneton.mk
	cd $(_VIEW_DIR_) 						; \
	$(SHELL) $(_VIEW_SH_) $*					; \
	cd ..

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

export- export:		kaneton.mk
	cd $(_EXPORT_DIR_)						; \
	$(SHELL) $(_EXPORT_SH_)						; \
	cd ..

export-%:		kaneton.mk
	cd $(_EXPORT_DIR_)						; \
	$(SHELL) $(_EXPORT_SH_)						; \
	cd ..

#
# ---------- /conditional -----------------------------------------------------
#

endif
