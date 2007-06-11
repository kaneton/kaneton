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
## updated       enguerrand raymond   [wed dec 20 15:04:48 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

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
	$(call env_launch,Makefile,$(SUBDIRS),,)

	$(call env_launch,Makefile,.,$(_LIBMIPSR10000_A_),)

$(_LIBMIPSR10000_A_):		$(LIBARC_LO)
	$(call env_remove,$(_LIBMIPSR10000_A_),)

	$(call env_static-library,$(_LIBMIPSR10000_A_),$(LIBARC_LO),"-crs")

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(_LIBMIPSR10000_A_),)

	$(call env_purge,)

proto:
	#$(call env_launch,Makefile,$(SUBDIRS),proto,)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
