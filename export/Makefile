##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/export
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Tue Oct 18 23:09:31 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

STAGES			:=		k1 k2 k3 k4 k5 k6 k7 k8 k9
DISTS			:=		$(patsubst %,%.tar.gz,$(STAGES))

#
# ---------- rules ------------------------------------------------------------
#

all:

clear:
	$(call remove,$(STAGES),-R)
	$(call remove,$(DISTS),)

	$(call purge)

proto:

dep:
