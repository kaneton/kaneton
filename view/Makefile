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

.PHONY:		all view build clear proto dep

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
					paper				\
					template			

#
# ---------- rules ------------------------------------------------------------
#

all:		build

view:
	$(call launch,$(_VIEW_SCRIPT_),$*,)				; \

build:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call purge,)

proto:

dep:
