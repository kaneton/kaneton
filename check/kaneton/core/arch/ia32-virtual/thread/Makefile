##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/arch/ia32-virtual/thread/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jul 19 20:22:08 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIST			:=		list

SUBDIRS			:=		$(call contents,$(LIST),)

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

THREAD_LO		:=		thread.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(THREAD_LO)

$(THREAD_LO):		libs
	$(call remove,$(THREAD_LO),)

	$(call archive,$(THREAD_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(THREAD_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
