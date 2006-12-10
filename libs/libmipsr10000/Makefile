##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/libs/libmipsr10000/Makefile
##
## created       enguerrand raymond   [wed oct 18 14:08:40 2006]
## updated       enguerrand raymond   [wed oct 18 14:36:51 2006]
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

LIBMIPSR10000_INCLUDE	:=		$(_LIBMIPSR10000_INCLUDE_DIR_)/libmipsr10000.h

#
# ---------- rules ------------------------------------------------------------
#

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

	$(call launch,Makefile,.,$(_LIBMIPSR10000_A_),)

$(_LIBMIPSR10000_A_):		$(LIBARC_LO)
	$(call remove,$(_LIBMIPSR10000_A_),)

	$(call static-library,$(_LIBMIPSR10000_A_),$(LIBARC_LO),"-crs")

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(_LIBMIPSR10000_A_),)

	$(call purge,)

proto:
	#$(call launch,Makefile,$(SUBDIRS),proto,)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
