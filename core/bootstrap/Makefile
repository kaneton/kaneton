##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/bootstrap
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:05:35 2005   mycure
## last update   Mon Jul  4 11:47:30 2005   mycure
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
# ---------- variables --------------------------------------------------------
#

BOOTSTRAP		=		bootstrap

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call make,arch,)

clear:
	$(call make,arch,clear)

	$(call remove,$(BOOTSTRAP),)

	$(call purge,)

proto:
	$(call make,arch,proto)

dep:
	$(call make,arch,dep)
