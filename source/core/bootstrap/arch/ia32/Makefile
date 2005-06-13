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
## last update   Sun Jun 12 22:34:45 2005   mycure
##

include			../../../../.kaneton.mk

BOOTSTRAP		=		bootstrap
BOOTSTRAP_ASM		=		bootstrap.asm

BOOTLOADER		=		bootloader
BOOTLOADER_O		=		bootloader.o
BOOTLOADER_C		=		bootloader.c

all:			
	$(call compile-asm,$(BOOTSTRAP),$(BOOTSTRAP_ASM),-f bin)

	$(call compile-c,$(BOOTLOADER_O),$(BOOTLOADER_C),)

	$(call dynamic-linker,$(BOOTLOADER),$(BOOTLOADER_O),		\
	  -T $(_LDS_DIR_)/bootstrap.lds)

distclean:
	$(call remove,$(BOOTSTRAP),)
	$(call remove,$(BOOTLOADER_O),)
	$(call remove,$(BOOTLOADER),)

	$(call purge,)
