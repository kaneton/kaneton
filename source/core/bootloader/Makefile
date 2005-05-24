##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/core/bootloader
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:05:35 2005   mycure
## last update   Fri May  6 00:40:44 2005   mycure
##

include			../../.common.mk

all:
	@cd arch							; \
	make								; \
	cd ..

distclean:
	@cd arch							; \
	make distclean							; \
	cd ..

	$(call rm,$(_BOOTLOADER_),)

proto:
	@cd arch							; \
	make proto							; \
	cd ..
