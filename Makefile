##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/Makefile
##
## created       julien quintard   [sun nov 20 14:06:01 2005]
## updated       matthieu bucchianeri   [mon jan 30 23:17:47 2006]
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

KANETON_SHELL		?=		/bin/bash

_SHELL_			?=		$(KANETON_SHELL)
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
  clean									\
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
	$(call change-directory,$(_ENV_DIR_),)				; \
	$(_SHELL_) clean.sh						; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		libs $(_SRC_DEP_)

CLEARDIRS		:=		libs view export check		\
					internship cheat

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
	$(call change-directory,$(_MBL_DIR_)/$(_MBL_),)			; \
	$(call launch,$(_MBL_).sh,build,)				; \
	$(call change-directory,$(_SRC_DIR_),)

install:		kaneton
	$(call change-directory,$(_MBL_DIR_)/$(_MBL_),)			; \
	$(call launch,$(_MBL_).sh,install,)				; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- check ------------------------------------------------------------
#

check:
	$(call makefile,$(_CHECK_DIR_),)

#
# ---------- view -------------------------------------------------------------
#

view- view:
	$(call change-directory,$(_VIEW_DIR_),)				; \
	$(call launch,$(_VIEW_SCRIPT_),$*,)				; \
	$(call change-directory,$(_SRC_DIR_),)

view-%:
	$(call change-directory,$(_VIEW_DIR_),)				; \
	$(call launch,$(_VIEW_SCRIPT_),$*,)				; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- information ------------------------------------------------------
#

info:
	$(call print,white,,)

	$(call print,blue,"--- ",--no-newline)
	$(call print,white,kaneton,)

	$(call print,white,,)

	$(call print,blue,"# ",--no-newline)
	$(call print,white,authors:,)
	$(call print,white,,)
	$(call print,white,     julien quintard - quinta_j@gmail.com,)
	$(call print,white,     jean-pascal billaud - billau_j@epita.fr,)

	$(call print,white,,)

	$(call print,blue,"# ",--no-newline)
	$(call print,white,assistants:,)
	$(call print,white,,)
	$(call print,white,     cedric aubouy - aubouy_c@epita.fr,)
	$(call print,white,     renaud lienhart - lienha_r@epita.fr,)

	$(call print,white,,)

	$(call print,blue,"# ",--no-newline)
	$(call print,white,kaneton reference:,)
	$(call print,white,,)
	$(call print,white,     matthieu bucchianeri - bucchi_m@epita.fr,)
	$(call print,white,     renaud voltz - voltz_r@epita.fr,)

	$(call print,white,,)

	$(call print,blue,"# ",--no-newline)
	$(call print,white,contributors:,)
	$(call print,white,,)
	$(call print,white,     christophe eymard - eymard_c@epita.fr,)
	$(call print,white,     solal jacob - jacob_s@epitech.net,)
	$(call print,white,     fabien le-mentec - le-men_f@epita.fr,)

	$(call print,white,,)

#
# ---------- dist -------------------------------------------------------------
#

dist:			export-dist

#
# ---------- export -----------------------------------------------------------
#

export- export:
	$(call change-directory,$(_EXPORT_DIR_),)			; \
	$(call launch,$(_EXPORT_SCRIPT_),,)				; \
	$(call change-directory,$(_SRC_DIR_),)

export-%:
	$(call change-directory,$(_EXPORT_DIR_),)			; \
	$(call launch,$(_EXPORT_SCRIPT_),$*,)				; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- /conditional -----------------------------------------------------
#

endif
