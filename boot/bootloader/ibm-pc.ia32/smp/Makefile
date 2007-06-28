##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-smp/Makefile
##
## created       julien quintard   [fri feb 11 02:05:35 2005]
## updated       matthieu bucchianeri   [wed nov  1 18:37:31 2006]
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
					pmode.c				\
					apic.c				\
					mp.c				\
					interrupt.c

BOOTLOADER_ASM		:=		multiboot.asm			\
					ap-boot.asm


BOOTLOADER_S		:=		entry.S

BOOTLOADER_O		:=		$(BOOTLOADER_C:.c=.o)		\
					$(BOOTLOADER_ASM:.asm=.o)	\
					$(BOOTLOADER_S:.S=.o)

BOOTLOADER_INCLUDE	:=		bootloader.h

#
# ---------- rules ------------------------------------------------------------
#

$(BOOTLOADER):		libs $(BOOTLOADER_O)
	$(call env_executable,$(BOOTLOADER),$(BOOTLOADER_O)			\
	         $(_LIBIA32_A_) $(_KLIBC_A_),				\
	         "--link-script $(_MACHDEP_LINK_DIR_)/bootloader.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

	$(call env_link,$(_BOOTLOADER_),arch/machdep/$(BOOTLOADER),)

ap-boot.o: ap-boot.asm
	$(call env_assemble-asm,ap-boot.o,ap-boot.asm,"--output-format elf")

libs:
	$(call env_launch,Makefile,$(_KLIBC_DIR_),,)

	$(call env_launch,Makefile,$(_LIBIA32_DIR_),,)

clear:
	$(call env_remove,$(BOOTLOADER_O),)
	$(call env_remove,ap-boot.o,)

	$(call env_remove,$(BOOTLOADER),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(BOOTLOADER_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(BOOTLOADER_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
