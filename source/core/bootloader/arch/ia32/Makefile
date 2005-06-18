##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/bootloader/arch/ia32
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:05:35 2005   mycure
## last update   Sat Jun 18 14:56:01 2005   mycure
##

#
# dependencies
#

include			../../../../.kaneton.mk

#
# directives
#

.PHONY:		clear proto dep

#
# variables
#

BOOTLOADER		:=		bootloader

BOOTLOADER_C		:=		bootloader.c			\
					cons.c				\
					entry.c				\
					init.c				\
					multiboot.c			\
					paging.c			\
					pmode.c				

BOOTLOADER_O		:=		$(BOOTLOADER_C:.c=.o)

BOOTLOADER_INCLUDE	:=		$(_MACHDEP_INCLUDE_DIR_)/bootloader.h

CFLAGS			+=		-D___bootloader

#
# rules
#

$(BOOTLOADER):		$(BOOTLOADER_O)
	$(call dynamic-linker,$(BOOTLOADER),$(BOOTLOADER_O)		\
	  $(_LIBC_A_), -T $(_LDS_DIR_)/bootloader.lds)

	$(call link,$(_BOOTLOADER_),arch/machdep/$(BOOTLOADER),)

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
# dependencies
#

include			./$(_MAKEFILE_MK_)
