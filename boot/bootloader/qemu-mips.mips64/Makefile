#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/enguerrand/...oot/bootloader/qemu-mips.mips64/Makefile
#
# created       enguerrand raymond   [sat oct 11 18:44:41 2008]
# updated       enguerrand raymond   [sat apr 11 02:29:05 2009]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		bootloader

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

BOOTLOADER		:=		bootloader

BOOTLOADER_C		:=		bootloader.c				\
					opmode.c				\
					mmu.c					\
					interrupt.c				\
					libc.c					\
					init.c					\
					exception.c

BOOTLOADER_S		:=		entry.S					\
					exception_handlers.S

BOOTLOADER_O		:=		$(BOOTLOADER_S:.S=.o)			\
					$(BOOTLOADER_C:.c=.o)

BOOTLOADER_INCLUDE	:=		$(_BOOTLOADER_DIR_)/bootloader.h	\
					$(_BOOTLOADER_DIR_)/libmips64.h		\
					$(_BOOTLOADER_DIR_)/libc.h

FIRMWARE		:=		firmware

FIRMWARE_BIN		:=		firmware.bin

FIRMWARE_C		:=		firmware.c

FIRMWARE_S		:=		firmware_entry.S

FIRMWARE_O		:=		$(FIRMWARE_S:.S=.o)		\
					$(FIRMWARE_C:.c=.o)

FIRMWARE_INCLUDE	:=		$(_FIRMWARE_DIR_)/firmware.h


#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(BOOTLOADER)	$(FIRMWARE)

$(BOOTLOADER):	$(BOOTLOADER_O)
	$(call env_executable,$(BOOTLOADER),				\
			      $(BOOTLOADER_O),				\
			      $(_BOOTLOADER_DIR_)/layout/bootloader.lyt,\
			      $(ENV_OPTION_NO_STANDARD))

$(FIRMWARE):	$(FIRMWARE_O)
	$(call env_executable,$(FIRMWARE),				\
			      $(FIRMWARE_O),				\
			      $(_FIRMWARE_DIR_)/layout/firmware.lyt,	\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(BOOTLOADER_O),)

	$(call env_remove,$(BOOTLOADER),)

	$(call env_remove,$(_BOOTLOADER_LO_),)

	$(call env_remove,$(BOOTLOADER_BIN),)

	$(call env_remove,$(FIRMWARE_O),)

	$(call env_remove,$(FIRMWARE),)

	$(call env_remove,$(_FIRMWARE_LO_),)

	$(call env_remove,$(FIRMWARE_BIN),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(BOOTLOADER_INCLUDE),)

	$(call env_prototypes,$(FIRMWARE_INCLUDE),)

	$(call env_headers,$(FIRMWARE_C),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(BOOTLOADER_C),)

	$(call env_headers,$(FIRMWARE_C),)

dependencies:
	$(call env_launch,Makefile,.,$(BOOTLOADER),)

	$(call env_launch,Makefile,.,$(FIRMWARE),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
