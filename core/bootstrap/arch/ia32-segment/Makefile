##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/bootstrap/arch/machdep/Makefile
##
## created       julien quintard   [fri feb 11 02:05:35 2005]
## updated       matthieu bucchianeri   [sun dec 18 12:47:41 2005]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

# bootstrap

BOOTSTRAP		=		bootstrap

BOOTSTRAP_ASM		=		bootstrap.asm

# bootloader

BOOTLOADER		=		bootloader

BOOTLOADER_C		=		bootloader.c

BOOTLOADER_O		=		$(BOOTLOADER_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(BOOTSTRAP):		$(BOOTLOADER)
	$(call assemble-asm,$(BOOTSTRAP),$(BOOTSTRAP_ASM),"--format bin")

$(BOOTLOADER):		$(BOOTLOADER_O)
	$(call executable,$(BOOTLOADER),$(BOOTLOADER_O),		\
	  "--lds $(_MACHDEP_LDS_DIR_)/bootstrap.lds")

clear:
	$(call remove,$(BOOTLOADER_O),)

	$(call remove,$(BOOTLOADER),)

	$(call remove,$(BOOTSTRAP),)

	$(call purge,)

proto:

dep:
