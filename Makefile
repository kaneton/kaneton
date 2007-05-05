##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/Makefile
##
## created       julien quintard   [sun nov 20 14:06:01 2005]
## updated       matthieu bucchianeri   [sat may  5 18:11:20 2007]
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
		dep build install check info dist backup		\
		view view- export export- cheat cheat-			\
		play play- record record-

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
  cheat cheat-								\
  play play-								\
  record record-							\
  clean									\
  view-% export-% cheat-% play-% record-%:	init
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

SUBDIRS			:=		libs apps drivers $(_SRC_DEP_)

CLEARDIRS		:=		libs view export check		\
					cheat				\
					$(_CTCOMPARE_DIR_)

#
# ---------- development ------------------------------------------------------
#

kaneton:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call launch,Makefile,$(CLEARDIRS),clear,)

purge:
	$(call purge,)

#
# ---------- prototypes -------------------------------------------------------
#

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

#
# ---------- dependencies -----------------------------------------------------
#

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)

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
	$(call launch,Makefile,$(_CHECK_DIR_),,)

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
# ---------- play -------------------------------------------------------------
#

play- play:
	$(call change-directory,$(_TRANSCRIPTS_DIR_),)			; \
	$(call launch,$(_PLAY_SCRIPT_),$*,)				; \
	$(call change-directory,$(_SRC_DIR_),)

play-%:
	$(call change-directory,$(_TRANSCRIPTS_DIR_),)			; \
	$(call launch,$(_PLAY_SCRIPT_),$*,)				; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- record -----------------------------------------------------------
#

record- record:
	$(call change-directory,$(_TRANSCRIPTS_DIR_),)			; \
	$(call launch,$(_RECORD_SCRIPT_),$*,)				; \
	$(call change-directory,$(_SRC_DIR_),)

record-%:
	$(call change-directory,$(_TRANSCRIPTS_DIR_),)			; \
	$(call launch,$(_RECORD_SCRIPT_),$*,)				; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- information ------------------------------------------------------
#

info:
	$(call print,white,,)

	$(call print,blue,"--- ",--no-newline)
	$(call print,white,http://www.kaneton.org,)

	$(call print,white,,)

#
# ---------- dist -------------------------------------------------------------
#

dist:			export-dist

#
# ---------- backup -----------------------------------------------------------
#

backup:			export-backup

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
# ---------- cheat ------------------------------------------------------------
#

cheat- cheat:
	$(call launch,Makefile,$(_CTCOMPARE_DIR_),,)

	$(call change-directory,$(_CHEAT_DIR_),)			; \
	$(call launch,$(_CHEAT_SCRIPT_),,)				; \
	$(call change-directory,$(_SRC_DIR_),)

cheat-%:
	$(call launch,Makefile,$(_CTCOMPARE_DIR_),,)

	$(call change-directory,$(_CHEAT_DIR_),)			; \
	$(call launch,$(_CHEAT_SCRIPT_),$*,)				; \
	$(call change-directory,$(_SRC_DIR_),)

#
# ---------- /conditional -----------------------------------------------------
#

endif
