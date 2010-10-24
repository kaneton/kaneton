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

.PHONY:		main view- view build clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		bibliography			\
					book				\
					exam				\
					feedback			\
					internship			\
					lecture				\
					package				\
					paper				\
					talk				\
					template			

#
# ---------- rules ------------------------------------------------------------
#

main:		build

view- view:
	$(call env_launch,$(_VIEW_SCRIPT_),$*,)

view-%:
	$(call env_launch,$(_VIEW_SCRIPT_),$*,)

build:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,build,)			; \
	done

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_purge,)

prototypes:

headers:
