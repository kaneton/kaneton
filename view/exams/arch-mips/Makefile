##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/exams/arch-mips/Makefile
##
## created       julien quintard   [sat dec  3 22:53:10 2005]
## updated       julien quintard   [thu jul 13 11:46:33 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		2006-partiel			\
					2006-exam			

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
