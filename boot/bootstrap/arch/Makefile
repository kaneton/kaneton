##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/bootstrap/arch/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       julien quintard   [thu jul 13 11:44:18 2006]
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

dep:
	$(call env_launch,Makefile,machdep,dep,)
