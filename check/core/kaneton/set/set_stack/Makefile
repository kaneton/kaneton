##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_ll/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [tue dec 20 22:21:46 2005]
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

SET_PIPE_LO		:=		set_stack.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(SET_PIPE_LO)

$(SET_PIPE_LO):	libs
	$(call remove,$(SET_PIPE_LO),)

	$(call archive,$(SET_PIPE_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(SET_PIPE_LO),)

	$(call purge,)

proto:

dep:
