##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/core/bootloader/Makefile
##
## created       julien quintard   [Fri Feb 11 02:05:35 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:15:39 2006]
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
	$(call makefile,arch,)

clear:
	$(call makefile,arch,clear)

	$(call remove,$(_BOOTLOADER_),)

	$(call purge,)

proto:
	$(call makefile,arch,proto)

dep:
	$(call makefile,arch,dep)
