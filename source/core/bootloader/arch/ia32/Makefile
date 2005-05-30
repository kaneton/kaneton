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
## last update   Sun May 29 00:38:41 2005   mycure
##

include			../../../../.common.mk

BOOTLOADER		=		bootloader

BOOTLOADER_INCLUDE	=		$(_INCLUDE_DIR_)/machdep/bootloader.h

BOOTLOADER_O		=		bootloader.o			\
					cons.o				\
					entry.o				\
					gdt.o				\
					multiboot.o			\
					paging.o			

CFLAGS			+=		-D___bootloader -D___kaneton

all:			$(BOOTLOADER_O)
	$(call ld,$(BOOTLOADER),$(BOOTLOADER_O) $(_LIBC_A_),		\
	  -T $(_LDS_DIR_)/bootloader.lds)

	@$(_LN_) arch/machdep/$(BOOTLOADER) $(_BOOTLOADER_)

distclean:
	$(call rm,$(BOOTLOADER_O),)
	$(call rm,$(BOOTLOADER),)

	@$(_PURGE_)

proto:
	$(call proto,$(BOOTLOADER_INCLUDE),)
