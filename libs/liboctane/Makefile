##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/libs/liboctane/Makefile
##
## created       Enguerrand RAYMOND   [sun oct 15 17:30:09 2006]
## updated       Enguerrand RAYMOND   [mon oct 16 20:56:05 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		libarc

LIBARC_LO		:=		libarc/libarc.lo

LIBOCTANE_INCLUDE	:=		$(_LIBOCTANE_INCLUDE_DIR_)/liboctane.h

#
# ---------- rules ------------------------------------------------------------
#

$(_LIBOCTANE_A_):		libs
	$(call remove,$(_LIBOCTANE_A_),)

	$(call static-library,$(_LIBOCTANE_A_),$(LIBARC_LO),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(_LIBOCTANE_A_),)

	$(call purge,)

proto:
	#$(call launch,Makefile,$(SUBDIRS),proto,)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
