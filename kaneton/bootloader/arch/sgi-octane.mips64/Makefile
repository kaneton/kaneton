##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-o2.mips64/Makefile
##
## created       enguerrand raymond   [wed oct 18 14:24:31 2006]
## updated       enguerrand raymond   [sun jan  7 00:30:54 2007]
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

BOOTLOADER_LO		:=		bootloader.lo

BOOTLOADER_C		:=		bootloader.c			\
					cons.c				\
					init.c

BOOTLOADER_O		:=		$(BOOTLOADER_C:.c=.o)		\
					$(BOOTLOADER_ASM:.asm=.o)	\
					$(BOOTLOADER_S:.S=.o)

BOOTLOADER_INCLUDE	:=		bootloader.h			\
					init.h

_CFLAGS_		=		$(_CFLAGS32_)
_LDFLAGS_		=		$(_LDFLAGS32_)


#
# ---------- rules ------------------------------------------------------------
#

$(BOOTLOADER_LO):	$(BOOTLOADER_O)
	$(call remove,$(BOOTLOADER_LO),)

	$(call archive,$(BOOTLOADER_LO),$(BOOTLOADER_O),)

clear:
	$(call remove,$(BOOTLOADER_O),)

	$(call remove,$(BOOTLOADER_LO),)

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