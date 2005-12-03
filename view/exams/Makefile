##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/exams/Makefile
##
## created       julien quintard   [sat dec  3 22:53:10 2005]
## updated       julien quintard   [sat dec  3 22:53:30 2005]
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
	$(call makefile,$(SUBDIRS),)

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call purge)

proto:

dep:
