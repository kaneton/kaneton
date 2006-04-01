##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/core/bootstrap/arch/ia32-virtual/Makefile
##
## created       julien quintard   [fri feb 11 02:05:35 2005]
## updated       julien quintard   [sat feb 25 14:43:22 2006]
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
	         "--ld-script $(_MACHDEP_LDS_DIR_)/bootstrap.lds"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

clear:
	$(call remove,$(BOOTLOADER_O),)

	$(call remove,$(BOOTLOADER),)

	$(call remove,$(BOOTSTRAP),)

	$(call purge,)

proto:

dep:
