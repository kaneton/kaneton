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
## last update   Tue Jun 14 15:20:35 2005   mycure
##

include			../../.kaneton.mk

SUBDIRS			=		arch				\
					segment				

KANETON			=		kaneton

KANETON_INCLUDE		=		$(_CORE_INCLUDE_DIR_)/kaneton/kaneton.h

KANETON_O		=		kaneton.o			\
					cons.o				

CFLAGS			+=		-D___kaneton -D___kernel

VERSION_C		=		version.c
VERSION_O		=		$(VERSION_C:.c=.o)

CONF			=		$(_CONF_DIR_)/$(USER)/$(USER).conf
TITLE			=		`cat $(CONF)			| \
					 grep -E "^TITLE = .*$$"	| \
					 cut -b 9-`
VERSION			=		`cat $(CONF)			| \
					 grep -E "^VERSION = .*$$"	| \
					 cut -b 11-`
DATE			=		`date`

all:			$(KANETON_O)
	@echo -n "const char version[] = \"$(TITLE)-$(VERSION)" > $(VERSION_C)
	@echo " $(DATE) $(USER)@$(HOSTNAME)\";" >> $(VERSION_C)
	$(call compile-c,$(VERSION_O),$(VERSION_C),)

	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_)							; \
	$(_CD_) ..							; \
	done

	$(call dynamic-linker,$(KANETON),	$(VERSION_O)		\
						$(KANETON_O)		\
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

	$(call remove,$(VERSION_O),)
	$(call remove,$(VERSION_C),)

	$(call purge,)

	$(call remove,$(KANETON),)

proto:
	$(call prototypes,$(KANETON_INCLUDE),)
