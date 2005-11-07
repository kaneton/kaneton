##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/drivers/dma
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:00:09 2005   mycure
## last update   Mon Nov  7 16:08:58 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

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

$(DMA):			libs $(DMA_O)
	$(call executable,$(DMA),$(DMA_O) $(_KLIBC_A_),			\
	  -T $(_MACHDEP_LDS_DIR_)/driver.lds)

libs:
	$(call makefile,$(_KLIBC_DIR_),)

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
