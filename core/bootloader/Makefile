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
## last update   Mon Nov  7 16:26:19 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call makefile,arch,)

clear:
	$(call makefile,arch,clear)

	$(call remove,$(_BOOTLOADER_),)

	$(call purge,)

proto:
	$(call makefile,arch,proto)

dep:
	$(call makefile,arch,dep)
