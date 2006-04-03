##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/task/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [tue apr  4 01:19:23 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../env/.env.mk

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

TASK_LO			:=		as.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(TASK_LO)

$(TASK_LO):		libs
	$(call remove,$(TASK_LO),)

	$(call archive,$(TASK_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(TASK_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)


dep:
