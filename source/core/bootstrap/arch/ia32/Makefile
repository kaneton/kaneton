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
## last update   Sat Jun 18 15:32:23 2005   mycure
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

# bootstrap

BOOTSTRAP		=		bootstrap

BOOTSTRAP_ASM		=		bootstrap.asm

# bootloader

BOOTLOADER		=		bootloader

BOOTLOADER_C		=		bootloader.c			

BOOTLOADER_O		=		$(BOOTLOADER_C:.c=.o)

#
# rules
#

$(BOOTSTRAP):		$(BOOTLOADER)
	$(call compile-asm,$(BOOTSTRAP),$(BOOTSTRAP_ASM),-f bin)

$(BOOTLOADER):
	$(call compile-c,$(BOOTLOADER_O),$(BOOTLOADER_C),)

	$(call dynamic-linker,$(BOOTLOADER),$(BOOTLOADER_O),		\
	  -T $(_LDS_DIR_)/bootstrap.lds)

clear:
	$(call remove,$(BOOTLOADER_O),)

	$(call remove,$(BOOTLOADER),)

	$(call remove,$(BOOTSTRAP),)

	$(call purge,)

proto:

dep:

