##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/feedbacks/Makefile
##
## created       julien quintard   [wed apr 12 07:57:31 2006]
## updated       julien quintard   [thu jul 13 11:46:03 2006]
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

SUBDIRS			:=		arch-ia32			\
					arch-mips			\
					kaneton				

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
