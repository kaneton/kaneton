##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/bootstrap/arch/ia32
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:05:35 2005   mycure
## last update   Sun Jun 12 21:15:26 2005   mycure
##

include			../../../../.kaneton.mk

BOOTSTRAP		=		bootstrap
BOOTSTRAP_ASM		=		bootstrap.asm

BOOTLOADER		=		bootloader
BOOTLOADER_O		=		bootloader.o
BOOTLOADER_C		=		bootloader.c

all:			
	$(call cc-asm-o,$(BOOTSTRAP),$(BOOTSTRAP_ASM),-f bin)

	$(call cc-c-o,$(BOOTLOADER_O),$(BOOTLOADER_C),)
	$(call ld,$(BOOTLOADER),$(BOOTLOADER_O),			\
	  -T $(_LDS_DIR_)/bootstrap.lds)

distclean:
	$(call rm,$(BOOTSTRAP),)
	$(call rm,$(BOOTLOADER_O),)
	$(call rm,$(BOOTLOADER),)

	@$(_PURGE_)
