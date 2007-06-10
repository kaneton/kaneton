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
## updated       enguerrand raymond   [mon jan 22 22:36:23 2007]
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

BOOTLOADER_LO		:=		bootloader.lo

BOOTLOADER_C		:=		bootloader.c			\
					cons.c				\
					init.c

BOOTLOADER_O		:=		$(BOOTLOADER_C:.c=.o)		\
					$(BOOTLOADER_ASM:.asm=.o)	\
					$(BOOTLOADER_S:.S=.o)

BOOTLOADER_INCLUDE	:=		bootloader.h			\
					init.h

#_CFLAGS_		=		$(_CFLAGS32_)
#_LDFLAGS_		=		$(_LDFLAGS32_)


#
# ---------- rules ------------------------------------------------------------
#

$(BOOTLOADER_LO):	$(BOOTLOADER_O)
	$(call env_env_remove,$(BOOTLOADER_LO),)

	$(call env_env_archive,$(BOOTLOADER_LO),$(BOOTLOADER_O),)

clear:
	$(call env_env_remove,$(BOOTLOADER_O),)

	$(call env_env_remove,$(BOOTLOADER_LO),)

	$(call env_env_purge,)

proto:
	$(call env_env_prototypes,$(BOOTLOADER_INCLUDE),)

dep:
	$(call env_env_remove,$(_DEPENDENCY_MK_),)

	$(call env_env_dependencies,$(BOOTLOADER_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)