##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/exams/arch-mips/Makefile
##
## created       julien quintard   [sat dec  3 22:53:10 2005]
## updated       julien quintard   [mon feb 20 23:00:00 2006]
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
	$(call makefile,$(SUBDIRS),)

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call purge)

proto:

dep:
