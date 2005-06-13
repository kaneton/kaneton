##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 12 22:52:41 2005   mycure
##

include			../../.kaneton.mk

SUBDIRS			=		arch				\
					segment				

KANETON			=		kaneton

KANETON_INCLUDE		=		$(_INCLUDE_DIR_)/kaneton.h

KANETON_O		=		kaneton.o

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(KANETON_O)
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_)							; \
	$(_CD_) ..							; \
	done

	$(call dynamic-linker,$(KANETON),	$(KANETON_O)		\
						$(_MACHDEP_A_)		\
						$(_SEGMENT_A_)		\
						$(_LIBC_A_),		\
	  -T $(_LDS_DIR_)/kaneton.lds)

distclean:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_) distclean						; \
	$(_CD_) ..							; \
	done

	$(call remove,$(KANETON_O),)

	$(call purge,)

	$(call remove,$(KANETON),)

proto:
	$(call prototypes,$(KANETON_INCLUDE),)
