##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/core/bootloader/arch/ia32-segment/Makefile
##
## created       julien quintard   [fri feb 11 02:05:35 2005]
## updated       julien quintard   [sat feb 25 14:41:49 2006]
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

$(BOOTLOADER):		libs $(BOOTLOADER_O)
	$(call executable,$(BOOTLOADER),$(BOOTLOADER_O)			\
	         $(_KLIBC_A_) $(_LIBIA32_A_),				\
	         "--ld-script $(_MACHDEP_LDS_DIR_)/bootloader.lds"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

	$(call link,$(_BOOTLOADER_),arch/machdep/$(BOOTLOADER),)

libs:
	$(call makefile,$(_KLIBC_DIR_),)

clear:
	$(call remove,$(BOOTLOADER_O),)

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
