#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...tloader/ibm-pc.ia32/educational/Makefile
#
# created       julien quintard   [tue jun 12 20:34:41 2007]
# updated       julien quintard   [tue jun 26 15:38:12 2007]
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
					libia32.c

BOOTLOADER_ASM		:=		multiboot.asm

BOOTLOADER_S		:=		entry.S

BOOTLOADER_O		:=		$(BOOTLOADER_C:.c=.o)		\
					$(BOOTLOADER_ASM:.asm=.o)	\
					$(BOOTLOADER_S:.S=.o)

BOOTLOADER_INCLUDE	:=		bootloader.h			\
					cons.h				\
					libia32.h

#
# ---------- rules ------------------------------------------------------------
#

main:			$(BOOTLOADER)

$(BOOTLOADER):		$(BOOTLOADER_O)
	$(call env_executable,$(BOOTLOADER),$(BOOTLOADER_O),		\
	         "--link-script $(_MACHDEP_LINK_DIR_)/bootloader.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

	$(call env_link,$(_BOOTLOADER_),arch/machdep/$(BOOTLOADER),)

clear:
	$(call env_remove,$(BOOTLOADER_O),)

	$(call env_remove,$(BOOTLOADER),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(BOOTLOADER_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(BOOTLOADER_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)
	$(call env_launch,Makefile,$(_LIBIA32_DIR_),,)

	$(call env_launch,Makefile,.,$(BOOTLOADER),)


#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
