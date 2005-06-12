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
## last update   Sun Jun 12 22:15:09 2005   mycure
##

include			../../.kaneton.mk

all:
	@$(CD) arch							; \
	$(MAKE)								; \
	$(CD) ..

distclean:
	@$(CD) arch							; \
	$(MAKE) distclean						; \
	$(CD) ..

	$(call remove,$(_BOOTLOADER_),)

	$(call purge,)

proto:
	@$(CD) arch							; \
	$(MAKE) proto							; \
	$(CD) ..
