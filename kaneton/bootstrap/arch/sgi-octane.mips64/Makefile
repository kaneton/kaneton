##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/kaneton/bootstrap/arch/sgi-octane.mips64/Makefile
##
## created       Enguerrand RAYMOND   [sun oct 15 17:31:41 2006]
## updated       Enguerrand RAYMOND   [sun oct 15 17:31:42 2006]
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

BOOTSTRAP_ASM		=		bootstrap.S

# bootloader

BOOTLOADER		=		bootloader

BOOTLOADER_C		=		bootloader.c

BOOTLOADER_O		=		$(BOOTLOADER_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(BOOTSTRAP):		#$(BOOTLOADER)
	$(call assemble-asm,$(BOOTSTRAP),$(BOOTSTRAP_ASM),"--format bin")

#$(BOOTLOADER):		$(BOOTLOADER_O)
	#$(call executable,$(BOOTLOADER),$(BOOTLOADER_O),		\
	 #        "--link-script $(_MACHDEP_LINK_DIR_)/bootstrap.lks"	\
          #       "--no-standard-include"				\
           #      "--no-standard-library")

clear:
	$(call remove,$(BOOTLOADER_O),)

	$(call remove,$(BOOTLOADER),)

	$(call remove,$(BOOTSTRAP),)

	$(call purge,)

proto:

dep:
