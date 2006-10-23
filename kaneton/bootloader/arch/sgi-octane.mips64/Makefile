##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-octane.mips64/Makefile
##
## created       enguerrand raymond   [wed oct 18 14:24:31 2006]
## updated       enguerrand raymond   [sun oct 22 23:40:44 2006]
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
					cons.c

BOOTLOADER_O		:=		$(BOOTLOADER_C:.c=.o)		\
					$(BOOTLOADER_S:.S=.o)

BOOTLOADER_INCLUDE	:=		bootloader.h

CFLAGS			+=		-D___bootloader

#
# ---------- rules ------------------------------------------------------------
#

$(BOOTLOADER):		libs	$(BOOTLOADER_O)
	$(call executable, $(BOOTLOADER), $(BOOTLOADER_O) $(_KLIBC_A_)	\
		$(_LIBMIPSR10000_A_),					\
		"--link-script $(_MACHDEP_LINK_DIR_)/bootloader.lks"	\
		"--no-standard-include"					\
		"--no-standard-library")

libs:
	$(call launch,Makefile,$(_KLIBC_DIR_),,)

	$(call launch,Makefile,$(_LIBMIPSR10000_DIR_),,)

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