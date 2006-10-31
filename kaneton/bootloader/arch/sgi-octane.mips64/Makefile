##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-octane.mips64/Makefile
##
## created       enguerrand raymond   [wed oct 18 14:24:31 2006]
## updated       enguerrand raymond   [sun oct 22 23:40:44 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

BOOTLOADER_LO		:=		bootloader.lo

BOOTLOADER_C		:=		bootloader.c

CONS_C			:=		cons.c

BOOTLOADER_O		:=		bootloader.o

CONS_O			:=		cons.o

BOOTLOADER_INCLUDE	:=		bootloader.h

CFLAGS			+=		-D___bootloader

#
# ---------- rules ------------------------------------------------------------
#

$(BOOTLOADER_LO):	$(CONS_O) $(BOOTLOADER_O)
	$(call remove,$(BOOTLOADER_LO),)

	$(call archive,$(BOOTLOADER_LO),$(BOOTLOADER_O) $(CONS_O),)

clear:
	$(call remove,$(BOOTLOADER_O),)

	$(call remove,$(CONS_O),)

	$(call remove,$(BOOTLOADER_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(BOOTLOADER_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(BOOTLOADER_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)