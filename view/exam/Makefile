##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/view/exams/Makefile
##
## created       julien quintard   [sat dec  3 22:53:10 2005]
## updated       matthieu bucchianeri   [fri feb  9 00:13:05 2007]
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

SUBDIRS			:=		arch-mips			\
					kernels

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
