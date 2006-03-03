##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/internships/Makefile
##
## created       renaud voltz   [thu feb  9 02:37:29 2006]
## updated       julien quintard   [mon feb 27 20:36:06 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		check

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call makefile,$(SUBDIRS),)

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call purge)

proto:

dep:
