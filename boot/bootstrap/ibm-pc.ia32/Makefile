#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/boot/bootstrap/ibm-pc.ia32/Makefile
#
# created       julien quintard   [tue jun 12 20:07:07 2007]
# updated       julien quintard   [tue jun 12 20:24:03 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

# bootstrap

BOOTSTRAP		=		bootstrap

BOOTSTRAP_ASM		=		bootstrap.asm

BOOTSTRAP_LYT		=		layout/bootstrap.lyt

# bootloader

BOOTLOADER		=		bootloader

BOOTLOADER_C		=		bootloader.c

BOOTLOADER_O		=		$(BOOTLOADER_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

main:			$(BOOTSTRAP)

$(BOOTSTRAP):		$(BOOTLOADER)
	$(call env_assemble-asm,$(BOOTSTRAP),$(BOOTSTRAP_ASM),		\
				$(ENV_OUTPUT_BINARY))

$(BOOTLOADER):		$(BOOTLOADER_O)
	echo SUCE
	$(call env_executable,$(BOOTLOADER),$(BOOTLOADER_O),		\
				$(BOOTSTRAP_LYT),$(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(BOOTLOADER_O),)

	$(call env_remove,$(BOOTLOADER),)

	$(call env_remove,$(BOOTSTRAP),)

	$(call env_purge,)

prototypes:

headers:
