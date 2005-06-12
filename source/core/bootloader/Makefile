##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/bootloader
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:05:35 2005   mycure
## last update   Sun Jun 12 21:14:39 2005   mycure
##

include			../../.kaneton.mk

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
