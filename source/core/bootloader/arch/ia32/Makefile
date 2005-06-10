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
## last update   Fri Jun 10 15:06:22 2005   mycure
##

include			../../../../.common.mk

BOOTLOADER		=		bootloader

VERSION_C		=		version.c
VERSION_O		=		$(VERSION_C:.c=.o)

BOOTLOADER_INCLUDE	=		$(_CORE_INCLUDE_DIR_)/kaneton/bootloader.h

BOOTLOADER_O		=		bootloader.o			\
					cons.o				\
					entry.o				\
					multiboot.o			\
					paging.o			\
					pmode.o				\
					segment.o			

CONF			=		$(_CONF_DIR_)/$(USER)/$(USER).conf
TITLE			=		`cat $(CONF)			| \
					 grep -E "^TITLE = .*$$"	| \
					 cut -b 9-`
VERSION			=		`cat $(CONF)			| \
					 grep -E "^VERSION = .*$$"	| \
					 cut -b 11-`
DATE			=		`date`

CFLAGS			+=		-D___bootloader -D___kaneton

all:			$(BOOTLOADER_O)
	@echo -n "const char version[] = \"$(TITLE)-$(VERSION)" > $(VERSION_C)
	@echo " $(DATE) $(USER)@$(HOSTNAME)\";" >> $(VERSION_C)
	$(call cc-c-o,$(VERSION_O),$(VERSION_C),)

	$(call ld,$(BOOTLOADER),$(VERSION_O) $(BOOTLOADER_O)		\
	  $(_LIBC_A_), -T $(_LDS_DIR_)/bootloader.lds)

	$(call link,$(_BOOTLOADER_),arch/machdep/$(BOOTLOADER),)

distclean:
	$(call rm,$(VERSION_C),)
	$(call rm,$(VERSION_O),)
	$(call rm,$(BOOTLOADER_O),)
	$(call rm,$(BOOTLOADER),)

	@$(_PURGE_)

proto:
	$(call proto,$(BOOTLOADER_INCLUDE),)
