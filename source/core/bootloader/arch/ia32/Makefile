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
## last update   Tue Jun 14 18:29:30 2005   mycure
##

include			../../../../.kaneton.mk

BOOTLOADER		=		bootloader

BOOTLOADER_INCLUDE	=		$(_MACHDEP_INCLUDE_DIR_)/bootloader.h

BOOTLOADER_O		=		as.o				\
					bootloader.o			\
					cons.o				\
					entry.o				\
					init.o				\
					multiboot.o			\
					paging.o			\
					pmode.o				\
					segment.o			

CFLAGS			+=		-D___bootloader -D___kaneton

all:			$(BOOTLOADER_O)
	$(call dynamic-linker,$(BOOTLOADER),$(BOOTLOADER_O)		\
	  $(_LIBC_A_), -T $(_LDS_DIR_)/bootloader.lds)

	$(call link,$(_BOOTLOADER_),arch/machdep/$(BOOTLOADER),)

distclean:
	$(call remove,$(BOOTLOADER_O),)
	$(call remove,$(BOOTLOADER),)

	$(call purge,)

proto:
	$(call prototypes,$(BOOTLOADER_INCLUDE),)
