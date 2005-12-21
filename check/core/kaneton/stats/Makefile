##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/core/kaneton/stats/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [tue dec 20 22:32:43 2005]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../env/.env.mk

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

STATS_LO		:=		stats.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(STATS_LO)

$(STATS_LO):		libs
	$(call remove,$(STATS_LO),)

	$(call archive,$(STATS_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(STATS_LO),)

	$(call purge,)

proto:

dep:
