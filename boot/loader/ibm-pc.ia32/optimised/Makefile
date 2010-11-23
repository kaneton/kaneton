#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/XYZ/...ot/loader/ibm-pc.ia32/optimised/Makefile
#
# created       julien quintard   [tue jun 12 20:34:41 2007]
# updated       julien quintard   [tue nov 23 20:27:38 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		loader

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

LOADER		:=		loader

LOADER_C		:=		apic.c				\
					bootloader.c			\
					cons.c				\
					init.c				\
					interrupt.c			\
					mp.c				\
					paging.c			\
					pmode.c				\
					libia32.c			\
					libc.c

LOADER_S		:=		multiboot.S			\
					entry.S

LOADER_ASM		:=		ap-boot.asm

LOADER_O		:=		$(LOADER_C:.c=.o)		\
					$(LOADER_S:.S=.o)		\
					$(LOADER_ASM:.asm=.o)

LOADER_INCLUDE	:=		$(_LOADER_DIR_)/bootloader.h \
					$(_LOADER_DIR_)/cons.h	\
					$(_LOADER_DIR_)/libia32.h	\
					$(_LOADER_DIR_)/libc.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(LOADER)

$(LOADER):		$(LOADER_O)
	$(call env_executable,$(_LOADER_),				\
			      $(LOADER_O),				\
			      $(_LOADER_DIR_)/layout/bootloader.lyt,\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(LOADER_O),)

	$(call env_remove,$(LOADER),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(LOADER_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(LOADER_C),)

dependencies:
	$(call env_launch,Makefile,.,$(LOADER),)

%.o:	%.asm
	nasm -f elf $< -o $@

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
