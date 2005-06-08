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
## last update   Wed Jun  8 16:45:38 2005   mycure
##

include			../../../../.common.mk

BOOTSTRAP		=		bootstrap
BOOTSTRAP_ASM		=		bootstrap.asm

BOOTLOADER		=		bootloader
BOOTLODER_C		=		bootloader.c

all:			
	$(call cc-asm-o,$(BOOTSTRAP_ASM),$(BOOTSTRAP),-f bin)

	$(call cc-c-o,$(BOOTLOADER_C),$(BOOTLOADER),)
	$(call ld,$(BOOTLOADER_C),$(BOOTLOADER),)

distclean:
	$(call rm,$(BOOTSTRAP),)

	@$(_PURGE_)
