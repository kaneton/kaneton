##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/bootloader/arch/ia32-virtual/Makefile
##
## created       julien quintard   [fri feb 11 02:05:35 2005]
## updated       matthieu bucchianeri   [thu aug 10 16:38:25 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

BOOTLOADER		:=		bootloader

BOOTLOADER_C		:=		bootloader.c			\
					cons.c				\
					init.c				\
					paging.c			\
					pmode.c

BOOTLOADER_ASM		:=		multiboot.asm

BOOTLOADER_S		:=		entry.S

BOOTLOADER_O		:=		$(BOOTLOADER_C:.c=.o)		\
					$(BOOTLOADER_ASM:.asm=.o)	\
					$(BOOTLOADER_S:.S=.o)

BOOTLOADER_INCLUDE	:=		bootloader.h

CFLAGS			+=		-D___bootloader

#
# ---------- rules ------------------------------------------------------------
#

libs:
	$(call env_launch,Makefile,$(_KLIBC_DIR_),,)
	$(call env_launch,Makefile,$(_LIBIA32_DIR_),,)

	$(call env_launch,Makefile,.,$(BOOTLOADER),)

$(BOOTLOADER):		$(BOOTLOADER_O)
	$(call env_executable,$(BOOTLOADER),$(BOOTLOADER_O)			\
	         $(_LIBIA32_A_) $(_KLIBC_A_),				\
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

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(BOOTLOADER_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
