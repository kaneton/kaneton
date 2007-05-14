##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/view/Makefile
##
## created       julien quintard   [sat oct 14 11:07:23 2006]
## updated       julien quintard   [sat oct 14 11:07:25 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		bibliography			\
					book				\
					curriculum			\
					exam				\
					feedback			\
					internship			\
					lecture				\
					logo				\
					lse				\
					paper				\
					template			

#
# ---------- rules ------------------------------------------------------------
#

view:
	$(call launch,$(_VIEW_SCRIPT_),$*,)				; \

all:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call purge,)

proto:

dep:
