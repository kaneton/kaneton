##
## copyright quintard julien
## 
## kaneton
## 
## .kaneton.mk
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:08:31 2005   mycure
## last update   Thu Jun 16 16:26:29 2005   mycure
##

#
# the source directory
#

_SRC_DIR_		=		___kaneton_sed___

#
# include .common.mk
#

include			.common.mk

#
# c compiler
#

CC			=		gcc

#
# binaries
#

_MAKE_			=		make
_RM_			=		rm -f
_PURGE_			=		$(RM) *~ .*~ \#* .\#*
_AR_			=		ar
_RANLIB_		=		ranlib
_CD_			=		cd
_CP_			=		cp
_LD_			=		ld
_NASM_			=		nasm
_CAT_			=		cat
_MCOPY_			=		mcopy -no
_MDEL_			=		mdel
_LN_			=		ln -s -f

#
# compiler and linker flags
#

CFLAGS			=		-nostdinc -nostdlib		\
					-fno-builtin			\
					-O0 $(_INCLUDES_)		\
					-D___little_endian		\
					-D___32bit			
LDFLAGS			=		-nostdinc -nostdlib		\
					$(_INCLUDES_)			

#
# traps to the pretty-printer
#

%.o:		%.asm
	$(call compile-asm,$@,$<,-f elf)

%.o:		%.S
	$(call compile-S,$@,$<,)

%.o:		%.c
	$(call compile-c,$@,$<,)

#
# pretty-printer defines
#
# 1:		action
# 2:		file
# 3:		identation
#

define pretty-printer-green
  echo -n -e '\E[;34m'"\033[1m[\033[0m"					; \
  echo -n -e '\E[;32m'"\033[1m$(1)\033[0m"				; \
  echo -n -e '\E[;34m'"\033[1m]\033[0m"					; \
  echo -e "$(3)\033[1m$(2)\033[0m"
endef

define pretty-printer-red
  echo -n -e '\E[;34m'"\033[1m[\033[0m"					; \
  echo -n -e '\E[;31m'"\033[1m$(1)\033[0m"				; \
  echo -n -e '\E[;34m'"\033[1m]\033[0m"					; \
  echo -e "$(3)\033[1m$(2)\033[0m"
endef

define pretty-printer-yellow
  echo -n -e '\E[;34m'"\033[1m[\033[0m"					; \
  echo -n -e '\E[;33m'"\033[1m$(1)\033[0m"				; \
  echo -n -e '\E[;34m'"\033[1m]\033[0m"					; \
  echo -e "$(3)\033[1m$(2)\033[0m"
endef

define pretty-printer-magenta
  echo -n -e '\E[;34m'"\033[1m[\033[0m"					; \
  echo -n -e '\E[;35m'"\033[1m$(1)\033[0m"				; \
  echo -n -e '\E[;34m'"\033[1m]\033[0m"					; \
  echo -e "$(3)\033[1m$(2)\033[0m"
endef

define pretty-printer-cyan
  echo -n -e '\E[;34m'"\033[1m[\033[0m"					; \
  echo -n -e '\E[;36m'"\033[1m$(1)\033[0m"				; \
  echo -n -e '\E[;34m'"\033[1m]\033[0m"					; \
  echo -e "$(3)\033[1m$(2)\033[0m"
endef

#
# pretty-printer wrapper
#
# $(1):		color
# $(2):		action
# $(3):		file
# $(4):		indentation
#

define pretty-printer
  $(call pretty-printer-$(1),$(2),$(3),$(4))
endef

#
# from c file to object file
#
# $(1):		object file
# $(2):		c file
# $(3):		advanced options
#

define compile-c
  @$(call pretty-printer,green,COMPILE-C,$(2),		)		; \
  $(CC) $(CFLAGS) $(_KANETON_OPTS_) $(3) -c $(2) -o $(1)
endef

#
# from S file to object file
#
# $(1):		object file
# $(2):		S file
# $(3):		advanced options
#

define compile-S
  @$(call pretty-printer,green,COMPILE-S,$(2),		)		; \
  $(CC) $(CFLAGS) $(_KANETON_OPTS_) $(3) -c $(2) -o $(1)
endef

#
# from asm file to object file
#
# $(1):		object file
# $(2):		asm file
# $(3):		advanced options
#

define compile-asm
  @$(call pretty-printer,green,COMPILE-ASM,$(2),		)	; \
  $(_NASM_) $(_KANETON_OPTS_) $(3) $(2) -o $(1)
endef

#
# from object files to archive
#
# $(1):		archive file
# $(2):		object files
# $(3):		advanced options
#

define archive
  @$(call pretty-printer,magenta,ARCHIVE,$(1),		)		; \
  $(_AR_) cq $(3) $(1) $(2)
endef

#
# from archives to archive
#
# $(1):		archive
# $(2):		archive files
# $(3):		advanced options
#

define archives
  @$(call pretty-printer,magenta,ARCHIVES,$(1),		)		; \
  (echo "CREATE $(1)"							; \
   for i in $(2) ; do							\
     echo "ADDLIB $$i"							; \
   done									; \
   echo "SAVE"								; \
   echo "END") | $(_AR_) -M $(3)
endef

#
# create a static library with index from an archive
#
# $(1):		archive file
# $(2):		advanced options
#

define static-linker
  @$(call pretty-printer,magenta,STATIC-LINKER,$(1),		)	; \
  $(_RANLIB_) $(2) $(1)
endef

#
# from objects files and/or archives and/or libraries to library
#
# $(1):		library file
# $(2):		objects files and/or archives and/or libraries
# $(3):		advanced options
#

define dynamic-linker
  @$(call pretty-printer,magenta,DYNAMIC-LINKER,$(1),	)		; \
  $(_LD_) $(LDFLAGS) $(3) -o $(1) $(2)
endef

#
# remove
#
# $(1):		files
# $(2):		advanced options
#

define remove
  @for i in $(1) ; do							\
    if [ -e $$i ] ; then						\
      $(call pretty-printer,red,REMOVE,$$i,		)		; \
    fi									; \
    $(_RM_) $(2) $$i							; \
  done
endef

#
# prototypes
#
# $(1):		file
# $(2):		advanced options
#

define prototypes
  @$(call pretty-printer,yellow,PROTOTYPES,$(1),		)	; \
  $(_PROTO_) $(2) $(1)
endef

#
# link
#
# $(1):		link created
# $(2):		destination
# $(3):		advances options
#

define link
  @$(call pretty-printer,yellow,LINK,$(1),			)	; \
  $(_LN_) $(3) $(2) $(1)
endef

#
# purge
#

define purge
  @$(call pretty-printer,yellow,PURGE,,)				; \
  $(_PURGE_)
endef
