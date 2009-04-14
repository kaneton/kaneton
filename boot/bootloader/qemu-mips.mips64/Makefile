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
# updated          [wed apr  8 16:08:06 2009]
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

BOOTLOADER_BIN		:=		bootloader.bin

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

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(BOOTLOADER)

$(BOOTLOADER):	$(BOOTLOADER_O)
	#$(call env_remove,$(_BOOTLOADER_LO_),)

	#$(call env_archive,$(_BOOTLOADER_LO_),$(BOOTLOADER_O),)

	$(call env_executable,$(BOOTLOADER),				\
			      $(BOOTLOADER_O),				\
			      $(_BOOTLOADER_DIR_)/layout/bootloader.lyt,\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(BOOTLOADER_O),)

	$(call env_remove,$(BOOTLOADER),)

	$(call env_remove,$(_BOOTLOADER_LO_),)

	$(call env_remove,$(BOOTLOADER_BIN),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(BOOTLOADER_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(BOOTLOADER_C),)

dependencies:
	$(call env_launch,Makefile,.,$(BOOTLOADER),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
