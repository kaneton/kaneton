##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/bootloader/Makefile
##
## created       julien quintard   [Fri Feb 11 02:05:35 2005]
## updated       julien quintard   [thu jul 13 11:44:02 2006]
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

	$(call remove,$(_BOOTLOADER_),)

	$(call purge,)

proto:
	$(call launch,Makefile,arch,proto,)

dep:
	$(call launch,Makefile,arch,dep,)
