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
## last update   Fri Jun 17 14:28:26 2005   mycure
##

include			../../.kaneton.mk

SUBDIRS			=		arch				\
					segment				\
					as				

KANETON			=		kaneton

KANETON_INCLUDE		=		$(_CORE_INCLUDE_DIR_)/kaneton/kaneton.h

KANETON_O		=		kaneton.o			\
					cons.o				

CFLAGS			+=		-D___kaneton -D___kernel

VERSION_H		=		$(_CORE_INCLUDE_DIR_)/version.h
VERSION_C		=		version.c
VERSION_O		=		$(VERSION_C:.c=.o)

DATE			=		`date`

all:			$(KANETON_O)
	@echo -n "" > $(VERSION_C)
	@echo "#include <libc.h>" >> $(VERSION_C)
	@echo "#include <kaneton.h>" >> $(VERSION_C)
	@echo "" >> $(VERSION_C)
	@echo -n "const char version[] = TITLE\"-\"VERSION" >> $(VERSION_C)
	@echo "\" $(DATE) $(USER)@$(HOSTNAME)\";" >> $(VERSION_C)
	$(call compile-c,$(VERSION_O),$(VERSION_C),)

	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_)							; \
	$(_CD_) ..							; \
	done

	$(call dynamic-linker,$(KANETON),	$(VERSION_O)		\
						$(KANETON_O)		\
						$(_MACHDEP_A_)		\
						$(_AS_A_)		\
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

	$(call remove,$(VERSION_O),)
	$(call remove,$(VERSION_C),)

	$(call purge,)

	$(call remove,$(KANETON),)

proto:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_) proto							; \
	$(_CD_) ..							; \
	done

	$(call prototypes,$(KANETON_INCLUDE),)
