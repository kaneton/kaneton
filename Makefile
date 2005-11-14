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
## last update   Sun Nov 13 12:46:55 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

-include		env/.env.mk

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

_SHELL_			?=		/bin/bash
_MAKE_			?=		$(MAKE)

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
	$(_MAKE_) -f Makefile $@
endif

#
# ---------- environment ------------------------------------------------------
#

init:
	cd env								; \
	$(_SHELL_) init.sh						; \
	cd ..

#
# ---------- conditional ------------------------------------------------------
#

ifeq ($(_SIGNATURE_),kaneton)

#
# ---------- environment ------------------------------------------------------
#

clean:
	$(call directory,$(_ENV_DIR_),)					; \
	$(_SHELL_) clean.sh						; \
	$(call directory,$(_SRC_DIR_),)

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		libs $(_SRC_DEP_)

CLEARDIRS		:=		libs view export

#
# ---------- development ------------------------------------------------------
#

kaneton:
	$(call makefile,$(SUBDIRS),)

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call makefile,$(CLEARDIRS),clear)

purge:
	$(call purge,)

#
# ---------- prototypes -------------------------------------------------------
#

proto:
	$(call makefile,$(SUBDIRS),proto)

#
# ---------- dependencies -----------------------------------------------------
#

dep:
	$(call makefile,$(SUBDIRS),dep)

#
# ---------- boot -------------------------------------------------------------
#

build:
	$(call directory,$(_MBL_DIR_)/$(_MBL_),)			; \
	$(call launch,$(_MBL_).sh,build,)				; \
	$(call directory,$(_SRC_DIR_),)

install:		kaneton
	$(call directory,$(_MBL_DIR_)/$(_MBL_),)			; \
	$(call launch,$(_MBL_).sh,install,)				; \
	$(call directory,$(_SRC_DIR_),)

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
	$(call directory,$(_VIEW_DIR_),)				; \
	$(call launch,$(_VIEW_SCRIPT_),$*,)				; \
	$(call directory,$(_SRC_DIR_),)

view-%:
	$(call directory,$(_VIEW_DIR_),)				; \
	$(call launch,$(_VIEW_SCRIPT_),$*,)				; \
	$(call directory,$(_SRC_DIR_),)

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
	$(call directory,$(_EXPORT_DIR_),)				; \
	$(call launch,$(_EXPORT_SCRIPT_),,)				; \
	$(call directory,$(_SRC_DIR_),)

export-%:
	$(call directory,$(_EXPORT_DIR_),)				; \
	$(call launch,$(_EXPORT_SCRIPT_),,)				; \
	$(call directory,$(_SRC_DIR_),)

#
# ---------- /conditional -----------------------------------------------------
#

endif
