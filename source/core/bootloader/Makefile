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
## last update   Sun Jun 12 22:28:46 2005   mycure
##

include			../../.kaneton.mk

all:
	@$(_CD_) arch							; \
	$(_MAKE_)							; \
	$(_CD_) ..

distclean:
	@$(_CD_) arch							; \
	$(_MAKE_) distclean						; \
	$(_CD_) ..

	$(call remove,$(_BOOTLOADER_),)

	$(call purge,)

proto:
	@$(_CD_) arch							; \
	$(_MAKE_) proto							; \
	$(_CD_) ..
