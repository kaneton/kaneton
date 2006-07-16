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

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call launch,Makefile,arch,,)

clear:
	$(call launch,Makefile,arch,clear,)

	$(call purge,)

proto:
	$(call launch,Makefile,arch,proto,)

dep:
	$(call launch,Makefile,arch,dep,)
