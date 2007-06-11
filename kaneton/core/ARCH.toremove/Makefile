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

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call env_launch,Makefile,machdep,,)

clear:
	$(call env_launch,Makefile,machdep,clear,)

	$(call env_purge,)

proto:
	$(call env_launch,Makefile,machdep,proto,)

headers:
	$(call env_launch,Makefile,machdep,dep,)
