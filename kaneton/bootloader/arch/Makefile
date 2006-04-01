##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/core/bootloader/arch/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:18:32 2006]
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
