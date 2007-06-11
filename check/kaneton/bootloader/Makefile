##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/bootloader/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jul 19 20:09:32 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIST			:=		list

SUBDIRS			:=		$(call env_contents,$(LIST),)

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

BOOTLOADER_LO		:=		bootloader.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(BOOTLOADER_LO)

$(BOOTLOADER_LO):		libs
	$(call env_remove,$(BOOTLOADER_LO),)

	$(call env_archive,$(BOOTLOADER_LO),$(MODULES),)

libs:
	$(call env_launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(BOOTLOADER_LO),)

	$(call env_purge,)

proto:
	$(call env_launch,Makefile,$(SUBDIRS),proto,)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
