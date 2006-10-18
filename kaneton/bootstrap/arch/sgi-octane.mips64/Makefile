##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/kaneton/bootstrap/arch/sgi-octane.mips64/Makefile
##
## created       enguerrand raymond   [wed oct 18 14:24:53 2006]
## updated       enguerrand raymond   [wed oct 18 14:24:53 2006]
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

BOOTSTRAP_O		=		$(BOOTSTRAP_ASM:.S=.o)

# bootloader

BOOTLOADER_O		=		bootloader.o

#
# ---------- rules ------------------------------------------------------------
#

$(BOOTSTRAP):
	$(call assemble-S,$(BOOTSTRAP_O),$(BOOTSTRAP_ASM),						\
		"--no-standard-include"									\
		"--no-standard-library")

	$(call executable, $(BOOTSTRAP), $(BOOTSTRAP_O) $(_MACHDEP_BOOTLOADER_DIR_)/$(BOOTLOADER_O),	\
		"--link-script $(_MACHDEP_LINK_DIR_)/bootstrap.lks"					\
		"--no-standard-include"									\
		"--no-standard-library")

clear:
	$(call remove,$(BOOTSTRAP_O),)

	$(call remove,$(BOOTSTRAP),)

	$(call purge,)

proto:

dep:
