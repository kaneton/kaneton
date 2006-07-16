##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/arch/Makefile
##
## created       matthieu bucchianeri   [tue jan 24 11:35:07 2006]
## updated       julien quintard   [thu jul 13 11:42:46 2006]
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
	$(call launch,Makefile,machdep,,)

clear:
	$(call launch,Makefile,machdep,clear,)

	$(call purge,)

proto:
	$(call launch,Makefile,machdep,proto,)

dep:
	$(call launch,Makefile,machdep,dep,)
