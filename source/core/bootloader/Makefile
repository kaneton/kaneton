##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:05:35 2005   mycure
## last update   Sun Jun 19 21:03:14 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- rules ------------------------------------------------------------
#

all:
	@$(_CD_) arch							; \
	$(_MAKE_)							; \
	$(_CD_) ..

clear:
	@$(_CD_) arch							; \
	$(_MAKE_) clear							; \
	$(_CD_) ..

	$(call remove,$(_BOOTLOADER_),)

	$(call purge,)

proto:
	@$(_CD_) arch							; \
	$(_MAKE_) proto							; \
	$(_CD_) ..

dep:
	@$(_CD_) arch							; \
	$(_MAKE_) dep							; \
	$(_CD_) ..
