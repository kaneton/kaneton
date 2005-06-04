##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/bootloader/arch/ia32
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:05:35 2005   mycure
## last update   Sat Jun  4 20:38:04 2005   mycure
##

include			../../../../.common.mk

BOOTLOADER		=		bootloader

VERSION_C		=		version.c
VERSION_O		=		$(VERSION_C:.c=.o)

BOOTLOADER_INCLUDE	=		$(_CORE_INCLUDE_DIR_)/kaneton/bootloader.h

BOOTLOADER_O		=		$(VERSION_O)			\
					bootloader.o			\
					cons.o				\
					entry.o				\
					multiboot.o			\
					paging.o			\
					pmode.o				

CONF			=		$(_CONF_DIR_)/$(USER)/$(USER).conf
TITLE			=		`cat $(CONF)			| \
					 grep -E "^TITLE = .*$$"	| \
					 cut -b 9-`
VERSION			=		`cat $(CONF)			| \
					 grep -E "^VERSION = .*$$"	| \
					 cut -b 11-`
DATE			=		`date`

CFLAGS			+=		-D___bootloader -D___kaneton

all:			version $(BOOTLOADER_O)
	$(call ld,$(BOOTLOADER),$(BOOTLOADER_O) $(_LIBC_A_),		\
	  -T $(_LDS_DIR_)/bootloader.lds)

	@$(_LN_) arch/machdep/$(BOOTLOADER) $(_BOOTLOADER_)

version:
	@echo -n "const char version[] = \"$(TITLE) $(VERSION) " > $(VERSION_C)
	@echo "$(DATE) $(USER)@$(HOSTNAME)\";" >> $(VERSION_C)
	$(call cc-c-o,$(VERSION_C),$(VERSION_O),)

distclean:
	$(call rm,$(VERSION_C),)
	$(call rm,$(BOOTLOADER_O),)
	$(call rm,$(BOOTLOADER),)

	@$(_PURGE_)

proto:
	$(call proto,$(BOOTLOADER_INCLUDE),)
