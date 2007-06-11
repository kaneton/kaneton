##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/bootstrap/Makefile
##
## created       julien quintard   [fri feb 11 02:05:35 2005]
## updated       julien quintard   [thu jul 13 11:44:26 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call env_launch,Makefile,arch,,)

clear:
	$(call env_launch,Makefile,arch,clear,)

	$(call env_purge,)

proto:
	$(call env_launch,Makefile,arch,proto,)

headers:
	$(call env_launch,Makefile,arch,dep,)
