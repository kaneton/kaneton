##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jul 19 20:16:22 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main build run libs clear prototypes dependencies

#
# ---------- variables --------------------------------------------------------
#

LIST			:=		list

SUBDIRS			:=		$(shell $(call env_pull,$(LIST),))

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

TEST_LO			:=		$(if $(_TESTSUITE_),$(_TEST_LO_),null)

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			$(TEST_LO)

# XXX hyper crade mais honnetement jmen branle vu la non conformite de check
null:
	touch $(_TEST_LO_)

$(_TEST_LO_):		libs
	$(call env_remove,$(_TEST_LO_),)

	$(call env_archive,$(_TEST_LO_),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

run:

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(_TEST_LO_),)

	$(call env_purge,)

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,dependencies,)		; \
	done
