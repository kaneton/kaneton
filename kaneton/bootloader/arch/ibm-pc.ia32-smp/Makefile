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

include			../../../../env/.env.mk

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

CFLAGS			+=		-D___bootloader

#
# ---------- rules ------------------------------------------------------------
#

$(BOOTLOADER):		libs $(BOOTLOADER_O)
	$(call executable,$(BOOTLOADER),$(BOOTLOADER_O)			\
	         $(_LIBIA32_A_) $(_KLIBC_A_),				\
	         "--link-script $(_MACHDEP_LINK_DIR_)/bootloader.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

	$(call link,$(_BOOTLOADER_),arch/machdep/$(BOOTLOADER),)

ap-boot.o: ap-boot.asm
	$(call assemble-asm,ap-boot.o,ap-boot.asm,"--output-format elf")

libs:
	$(call launch,Makefile,$(_KLIBC_DIR_),,)

	$(call launch,Makefile,$(_LIBIA32_DIR_),,)

clear:
	$(call remove,$(BOOTLOADER_O),)
	$(call remove,ap-boot.o,)

	$(call remove,$(BOOTLOADER),)

	$(call purge,)

proto:
	$(call prototypes,$(BOOTLOADER_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(BOOTLOADER_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
