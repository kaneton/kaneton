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
## last update   Sun Jul  3 19:39:48 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIBDATA			:=		libdata.a

LIBDATA_C		:=		dump.c				\
					alloc.c				\
					sum2.c				

LIBDATA_O		:=		$(LIBDATA_C:.c=.o)

LIBDATA_INCLUDE		:=		$(_LIBS_DIR_)/klibc/include/libdata/libdata.h

#
# ---------- rules ------------------------------------------------------------
#

$(LIBDATA):		$(LIBDATA_O)
	$(call remove,$(LIBDATA),)

	$(call archive,$(LIBDATA),$(LIBDATA_O),)

	$(call static-library,$(LIBDATA),)

clear:
	$(call remove,$(LIBDATA_O),)

	$(call remove,$(LIBDATA),)

	$(call purge,)

proto:
	$(call prototypes,$(LIBDATA_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(LIBDATA_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
