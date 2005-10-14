##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/services/dma
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:00:09 2005   mycure
## last update   Thu Oct 13 22:07:37 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

DMA			:=		dma

DMA_C			:=		dma.c

DMA_O			:=		$(DMA_C:.c=.o)

DMA_INCLUDE		:=		

#
# ---------- rules ------------------------------------------------------------
#

$(DMA):			$(DMA_O)
	$(call executable,$(DMA),$(DMA_O) $(_LIBC_A_),			\
	  -T $(_LDS_DIR_)/service.lds)

clear:
	$(call remove,$(DMA_O),)
	$(call remove,$(DMA),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(DMA_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
