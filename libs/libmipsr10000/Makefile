##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/libs/libmipsR10000/Makefile
##
## created       enguerrand raymond   [wed oct 18 14:08:40 2006]
## updated       enguerrand raymond   [wed oct 18 14:08:42 2006]
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
