#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/boot/loader/qemu-mips.mips64/Makefile
#
# created       enguerrand raymond   [sat oct 11 18:44:41 2008]
# updated       julien quintard   [sat may  2 00:02:40 2009]
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

BOOTLOADER_C		:=		bootloader.c				\
					opmode.c				\
					mmu.c					\
					interrupt.c				\
					libc.c					\
					init.c					\
					exception.c				\
					cons.c					\
					uart.c

BOOTLOADER_S		:=		entry.S

BOOTLOADER_O		:=		$(BOOTLOADER_S:.S=.o)			\
					$(BOOTLOADER_C:.c=.o)

BOOTLOADER_INCLUDE	:=		$(_LOADER_DIR_)/bootloader.h		\
					$(_LOADER_DIR_)/libmips64.h		\
					$(_LOADER_DIR_)/libc.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_LOADER_)

$(_LOADER_):		$(BOOTLOADER_O)
	$(call env_executable,$(_LOADER_),				\
			      $(BOOTLOADER_O),				\
			      $(_LOADER_DIR_)/layout/bootloader.lyt,	\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(BOOTLOADER_O),)

	$(call env_remove,$(_LOADER_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(BOOTLOADER_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(BOOTLOADER_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
