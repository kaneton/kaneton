##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/libs/klibc/libdata
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:05:40 2005   mycure
## last update   Mon Nov  7 15:48:40 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIBDATA_LO		:=		libdata.lo

LIBDATA_C		:=		dump.c				\
					alloc.c				\
					sum2.c				

LIBDATA_O		:=		$(LIBDATA_C:.c=.o)

LIBDATA_INCLUDE		:=		$(_KLIBC_INCLUDE_DIR_)/libdata/libdata.h

#
# ---------- rules ------------------------------------------------------------
#

$(LIBDATA_LO):		$(LIBDATA_O)
	$(call remove,$(LIBDATA_LO),)

	$(call archive,$(LIBDATA_LO),$(LIBDATA_O),)

clear:
	$(call remove,$(LIBDATA_O),)

	$(call remove,$(LIBDATA_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(LIBDATA_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(LIBDATA_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
