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

include			../environment/env.mk

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
	$(call env_launch,$(_VIEW_SCRIPT_),$*,)				; \

build:
	$(call env_launch,Makefile,$(SUBDIRS),,)

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_purge,)

proto:

dep:
