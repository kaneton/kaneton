#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kan...ootloader/ibm-pc.ia32/optimised/Makefile
#
# created       julien quintard   [tue jun 12 20:34:41 2007]
# updated       matthieu bucchianeri   [mon dec 24 18:11:11 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		bootloader

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

BOOTLOADER		:=		bootloader

BOOTLOADER_C		:=		apic.c				\
					bootloader.c			\
					cons.c				\
					init.c				\
					interrupt.c			\
					mp.c				\
					paging.c			\
					pmode.c				\
					libia32.c			\
					libc.c

BOOTLOADER_S		:=		multiboot.S			\
					entry.S

BOOTLOADER_ASM		:=		ap-boot.asm

BOOTLOADER_O		:=		$(BOOTLOADER_C:.c=.o)		\
					$(BOOTLOADER_S:.S=.o)		\
					$(BOOTLOADER_ASM:.asm=.o)

BOOTLOADER_INCLUDE	:=		$(_BOOTLOADER_DIR_)/bootloader.h \
					$(_BOOTLOADER_DIR_)/cons.h	\
					$(_BOOTLOADER_DIR_)/libia32.h	\
					$(_BOOTLOADER_DIR_)/libc.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(BOOTLOADER)

$(BOOTLOADER):		$(BOOTLOADER_O)
	$(call env_executable,$(_BOOTLOADER_),				\
			      $(BOOTLOADER_O),				\
			      $(_BOOTLOADER_DIR_)/layout/bootloader.lyt,\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(BOOTLOADER_O),)

	$(call env_remove,$(BOOTLOADER),)

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
