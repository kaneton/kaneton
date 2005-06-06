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
## last update   Sat Jun  4 22:19:02 2005   mycure
##

include			../../../../.common.mk

BOOTSTRAP		=		bootstrap

BOOTSTRAP_ASM		=		bootstrap.asm

all:			
	$(call cc-asm-o,$(BOOTSTRAP_ASM),$(BOOTSTRAP),-f bin)

distclean:
	$(call rm,$(BOOTSTRAP),)

	@$(_PURGE_)
