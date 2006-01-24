##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/core/kaneton/arch/Makefile
##
## created       matthieu bucchianeri   [tue jan 24 11:35:07 2006]
## updated       matthieu bucchianeri   [tue jan 24 11:35:09 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call makefile,machdep,)

clear:
	$(call makefile,machdep,clear)

	$(call purge,)

proto:
	$(call makefile,machdep,proto)

dep:
	$(call makefile,machdep,dep)
