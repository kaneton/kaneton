##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/exams/Makefile
##
## created       julien quintard   [sat dec  3 22:53:10 2005]
## updated       julien quintard   [thu jul 13 11:46:43 2006]
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

SUBDIRS			:=		arch-mips			

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call purge)

proto:

dep:
