##
## copyright quintard julien
## 
## kaneton
## 
## common.mk
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:08:31 2005   mycure
## last update   Wed Jun  8 17:23:40 2005   mycure
##

#
# kaneton directories
#

_SRC_DIR_		=		___kaneton_sed___
_CORE_DIR_		=		$(_SRC_DIR_)/core
_CORE_BOOTSTRAO_DIR_	=		$(_SRC_DIR_)/core/bootstrap
_CORE_BOOTLOADER_DIR_	=		$(_SRC_DIR_)/core/bootloader
_CORE_INCLUDE_DIR_	=		$(_SRC_DIR_)/core/include
_CORE_KANETON_DIR_	=		$(_SRC_DIR_)/core/kaneton
_MACHDEP_DIR_		=		$(_CORE_KANETON_DIR_)/arch/machdep
_MACHDEP_INCLUDE_DIR_	=		$(_CORE_INCLUDE_DIR_)/arch/machdep
_DIST_DIR_		=		$(_SRC_DIR_)/dist
_ENV_DIR_		=		$(_SRC_DIR_)/env
_LDS_DIR_		=		$(_SRC_DIR_)/lds/arch/machdep
_LIB_DIR_		=		$(_SRC_DIR_)/libs
_LIB_INCLUDE_DIR_	=		$(_LIB_DIR_)
_INCLUDES_		=		-I$(_CORE_INCLUDE_DIR_)		\
					-I$(_LIB_INCLUDE_DIR_)
_CONF_DIR_		=		$(_SRC_DIR_)/conf
_UTILS_DIR_		=		$(_SRC_DIR_)/utils

#
# kaneton utils
#

_PROTO_			=		$(_UTILS_DIR_)/mkp.pl
_GRUB_			=		$(_UTILS_DIR_)/grub.sh
_LILO_			=		$(_UTILS_DIR_)/lilo.sh

#
# kaneton binaries
#

_BOOTSTRAP_		=		$(_CORE_BOOTSTRAP_DIR_)/bootstrap
_BOOTLOADER_		=		$(_CORE_BOOTLOADER_DIR_)/bootloader
_KANETON_		=		$(_CORE_KANETON_DIR_)/kaneton

#
# kaneton libraries
#

_LIBC_A_		=		$(_LIB_DIR_)/libc.a
_LIBC_H_		=		$(_LIB_DIR_)/libc.h

_CRT_A_			=		$(_LIB_DIR_)/crt.a

_MACHDEP_A_		=		$(_MACHDEP_DIR_)/machdep.a
_AS_A_			=		$(_CORE_KANETON_DIR_)/as/as.a
_CPU_A_			=		$(_CORE_KANETON_DIR_)/cpu/cpu.a
_MAP_A_			=		$(_CORE_KANETON_DIR_)/map/map.a
_MSG_A_			=		$(_CORE_KANETON_DIR_)/msg/msg.a
_REGION_A_		=		$(_CORE_KANETON_DIR_)/region/region.a
_SCHED_A_		=		$(_CORE_KANETON_DIR_)/sched/sched.a
_SEGMENT_A_		=		$(_CORE_KANETON_DIR_)/segment/segment.a
_SET_A_			=		$(_CORE_KANETON_DIR_)/set/set.a
_TASK_A_		=		$(_CORE_KANETON_DIR_)/task/task.a
_THREAD_A_		=		$(_CORE_KANETON_DIR_)/thread/thread.a
_TIME_A_		=		$(_CORE_KANETON_DIR_)/time/time.a
_TRAP_A_		=		$(_CORE_KANETON_DIR_)/trap/trap.a

#
# include kaneton makefile dependency
#

include			$(_SRC_DIR_)/.kaneton.mk

#
# traps to the pretty-printer
#

%.o:		%.asm
	$(call cc-asm-o,$@,$<,-f elf)

%.o:		%.S
	$(call cc-S-o,$@,$<,)

%.o:		%.c
	$(call cc-c-o,$@,$<,)

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
# from C file to object file
#
# $(1):		object file
# $(2):		C file
# $(3):		advanced options
#

define cc-c-o
  @$(call pretty-printer,green,CC,$(2),			)		; \
  $(CC) $(CFLAGS) $(_KANETON_OPTS_) $(3) -c $(2) -o $(1)
endef

#
# from S file to object file
#
# $(1):		object file
# $(2):		S file
# $(3):		advanced options
#

define cc-S-o
  @$(call pretty-printer,green,CC,$(2),			)		; \
  $(CC) $(CFLAGS) $(_KANETON_OPTS_) $(3) -c $(2) -o $(1)
endef

#
# from asm file to object file
#
# $(1):		object file
# $(2):		asm file
# $(3):		advanced options
#

define cc-asm-o
  @$(call pretty-printer,green,NASM,$(2),			)	; \
  $(_NASM_) $(_KANETON_OPTS_) $(3) $(2) -o $(1)
endef

#
# from object files to archive
#
# $(1):		archive file
# $(2):		object files
# $(3):		advanced options
#

define ar-o-a
  @$(call pretty-printer,red,AR,$(1),			)		; \
  $(_AR_) cq $(3) $(1) $(2)
endef

#
# from archives to archive
#
# $(1):		archive
# $(2):		archive files
# $(3):		advanced options
#

define ar-a-a
  @$(call pretty-printer,red,AR,$(1),			)		; \
  (echo "CREATE $(1)"							; \
   for i in $(2) ; do							\
     echo "ADDLIB $$i"							; \
   done									; \
   echo "SAVE"								; \
   echo "END") | $(_AR_) -M $(3)
endef

#
# archive
#
# $(1):		archive file
# $(2):		advanced options
#

define ranlib
  @$(call pretty-printer,red,RANLIB,$(1),		)		; \
  $(_RANLIB_) $(2) $(1)
endef

#
# from objects files and/or archives and/or libraries to library
#
# $(1):		library file
# $(2):		objects files and/or archives and/or libraries
# $(3):		advanced options
#

define ld
  @$(call pretty-printer,red,LD,$(1),			)		; \
  $(_LD_) $(LDFLAGS) $(3) -o $(1) $(2)
endef

#
# remove
#
# $(1):		files
# $(2):		advanced options
#

define rm
  @for i in $(1) ; do							\
    $(call pretty-printer,magenta,RM,$$i,			)	; \
    $(_RM_) $(2) $$i							; \
  done
endef

#
# prototypes
#
# $(1):		file
# $(2):		advanced options
#

define proto
  @$(call pretty-printer,yellow,PROTO,$(1),			)	; \
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
  echo "---------- link from $(1) to $(2) [$(3)] in $(PWD)"

  @$(call pretty-printer,yellow,LINK,$(1),			)	; \
  $(_LN_) $(3) $(2) $(1)
endef
