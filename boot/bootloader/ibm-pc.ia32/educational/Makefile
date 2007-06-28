#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kan...tloader/ibm-pc.ia32/educational/Makefile
#
# created       julien quintard   [tue jun 12 20:34:41 2007]
# updated       matthieu bucchianeri   [thu jun 28 17:29:23 2007]
#

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

BOOTLOADER_C		:=		bootloader.c			\
					cons.c				\
					init.c				\
					paging.c			\
					pmode.c				\
					libia32.c			\
					libc.c

BOOTLOADER_ASM		:=		multiboot.asm

BOOTLOADER_S		:=		entry.S

BOOTLOADER_O		:=		$(BOOTLOADER_C:.c=.o)		\
					$(BOOTLOADER_ASM:.asm=.o)	\
					$(BOOTLOADER_S:.S=.o)

BOOTLOADER_INCLUDE	:=		$(_BOOTLOADER_DIR_)/bootloader.h \
					$(_BOOTLOADER_DIR_)/cons.h	\
					$(_BOOTLOADER_DIR_)/libia32.h	\
					$(_BOOTLOADER_DIR_)/libc.h

#
# ---------- rules ------------------------------------------------------------
#

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


#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
