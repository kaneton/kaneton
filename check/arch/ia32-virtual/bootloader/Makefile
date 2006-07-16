##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/arch/ia32-virtual/bootloader/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:41:29 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIST			:=		list

SUBDIRS			:=		$(call contents,$(LIST),)

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

BOOTLOADER_LO		:=		bootloader.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(BOOTLOADER_LO)

$(BOOTLOADER_LO):		libs
	$(call remove,$(BOOTLOADER_LO),)

	$(call archive,$(BOOTLOADER_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(BOOTLOADER_LO),)

	$(call purge,)

proto:

dep:
