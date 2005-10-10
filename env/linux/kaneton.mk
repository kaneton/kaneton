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
## last update   Mon Oct 10 09:14:28 2005   mycure
## last update   Thu Sep 15 16:58:31 2005   mycure
##

#
# ---------- informations -----------------------------------------------------
#
# you generally do not need to modify the kaneton directories,
# makefiles dependency, kaneton utils, kaneton binaries, kaneton
# libraries and user configuration addons.
#
# nevertheless, you can change the c compiler, compiler and linker
# flags and the binaries.
#
# if needed also modify the makefile functions to match with your binaries.
#

#
# ---------- directives -------------------------------------------------------
#

.SILENT:

#
# ---------- shell ------------------------------------------------------------
#

SHELL			:=		/bin/sh

_SHELL_			:=		$(SHELL)

#
# ---------- kaneton directories ----------------------------------------------
#

_SRC_DIR_		:=		___kaneton_sed___

_CORE_DIR_		:=		$(_SRC_DIR_)/core
_CORE_BOOTSTRAP_DIR_	:=		$(_SRC_DIR_)/core/bootstrap
_CORE_BOOTLOADER_DIR_	:=		$(_SRC_DIR_)/core/bootloader
_CORE_INCLUDE_DIR_	:=		$(_SRC_DIR_)/core/include
_CORE_KANETON_DIR_	:=		$(_SRC_DIR_)/core/kaneton

_MACHDEP_KANETON_DIR_	:=		$(_CORE_KANETON_DIR_)/arch/machdep
_MACHDEP_INCLUDE_DIR_	:=		$(_CORE_INCLUDE_DIR_)/arch/machdep

_DIST_DIR_		:=		$(_SRC_DIR_)/dist
_ENV_DIR_		:=		$(_SRC_DIR_)/env
_LDS_DIR_		:=		$(_SRC_DIR_)/lds/arch/machdep
_CONF_DIR_		:=		$(_SRC_DIR_)/conf
_UTILS_DIR_		:=		$(_SRC_DIR_)/utils
_PAPERS_DIR_		:=		$(_SRC_DIR_)/papers

_LIBS_DIR_		:=		$(_SRC_DIR_)/libs
_LIBS_INCLUDE_DIR_	:=		$(_LIBS_DIR_)

_INCLUDES_		:=		-I$(_CORE_INCLUDE_DIR_)		\
					-I$(_LIBS_INCLUDE_DIR_)

#
# ---------- makefiles dependency ---------------------------------------------
#

_MAKEFILE_MK_		:=		.makefile.mk

#
# ---------- kaneton utils ----------------------------------------------------
#

_PROTO_			:=		$(_UTILS_DIR_)/mkp.py
_VIEWER_		:=		$(_PAPERS_DIR_)/viewer.sh

#
# ---------- kaneton binaries -------------------------------------------------
#

_BOOTSTRAP_		:=		$(_CORE_BOOTSTRAP_DIR_)/bootstrap
_BOOTLOADER_		:=		$(_CORE_BOOTLOADER_DIR_)/bootloader
_KANETON_		:=		$(_CORE_KANETON_DIR_)/kaneton

#
# ---------- kaneton libraries ------------------------------------------------
#

_LIBC_A_		:=		$(_LIBS_DIR_)/libc.a
_LIBC_H_		:=		$(_LIBS_DIR_)/libc.h

_CRT_A_			:=		$(_LIBS_DIR_)/crt.a

_AS_LO_			:=		$(_CORE_KANETON_DIR_)/as/as.lo
_DEBUG_LO_		:=		$(_CORE_KANETON_DIR_)/debug/debug.lo
_ID_LO_			:=		$(_CORE_KANETON_DIR_)/id/id.lo
_MACHDEP_LO_		:=		$(_MACHDEP_KANETON_DIR_)/machdep.lo
_SEGMENT_LO_		:=		$(_CORE_KANETON_DIR_)/segment/segment.lo
_SET_LO_		:=		$(_CORE_KANETON_DIR_)/set/set.lo
_STATS_LO_		:=		$(_CORE_KANETON_DIR_)/stats/stats.lo

#
# ---------- user configuration addons ----------------------------------------
#

include			$(_CONF_DIR_)/$(USER)/$(USER).conf

#
# ---------- c compiler -------------------------------------------------------
#

CC			:=		gcc

#
# ---------- compiler and linker flags ----------------------------------------
#

CFLAGS			:=		-D___kaneton			\
					-nostdinc			\
					-nostdlib			\
					-fno-builtin			\
					-O0				\
					$(_INCLUDES_)			\
					$(_CFLAGS_)			\
					$(_KFLAGS_)			

LDFLAGS			:=		-nostdinc			\
					-nostdlib			\
					$(_INCLUDES_)			\
					$(_LDFLAGS_)			

CPPFLAGS		:=		$(_CPPFLAGS_)			

MAKEFLAGS		:=		$(_MAKEFLAGS_)			

#
# ---------- binaries ---------------------------------------------------------
#

_MAKE_			:=		make
_RM_			:=		rm -f
_PURGE_			:=		$(RM) *~ .*~ \#* .\#*
_AR_			:=		ar cq
_RANLIB_		:=		ranlib
_CD_			:=		cd
_LD_			:=		ld
_NASM_			:=		nasm
_LN_			:=		ln -s -f
_TOUCH_			:=		touch
_WC_			:=		wc
_TAIL_			:=		tail
_TAR_			:=		tar
_LATEX_			:=		latex
_DVIPS_			:=		dvips
_PDFLATEX_		:=		pdflatex

#
# ---------- traps ------------------------------------------------------------
#

%.o:			%.asm
	$(call compile-asm,$@,$<,-f elf)

%.o:			%.S
	$(call compile-S,$@,$<,)

%.o:			%.c
	$(call compile-c,$@,$<,)

#
# ---------- functions --------------------------------------------------------
#

#
# print functions
#
# 1:		text
# 2:		advanced options
#

define print-black
  echo -e $(2) '\E[;30m'"\033[1m$(1)\033[0m"
endef

define print-red
  echo -e $(2) '\E[;31m'"\033[1m$(1)\033[0m"
endef

define print-green
  echo -e $(2) '\E[;32m'"\033[1m$(1)\033[0m"
endef

define print-yellow
  echo -e $(2) '\E[;33m'"\033[1m$(1)\033[0m"
endef

define print-blue
  echo -e $(2) '\E[;34m'"\033[1m$(1)\033[0m"
endef

define print-magenta
  echo -e $(2) '\E[;35m'"\033[1m$(1)\033[0m"
endef

define print-cyan
  echo -e $(2) '\E[;36m'"\033[1m$(1)\033[0m"
endef

define print-white
  echo -e $(2) '\E[;37m'"\033[1m$(1)\033[0m"
endef

define print-non-color
  echo $(2) "$(1)"
endef

#
# print wrapper functions
#
# 1:		color
# 2:		text
# 3:		advanced options
#

ifeq ($(_DISPLAY_),color)			# if the user wants to display
						# the text with color

define print
  $(call print-$(1),$(2),$(3))
endef

else						# if not ...

define print
  $(call print-non-color,$(2),$(3))
endef

endif

#
# pretty-printer functions
#
# 1:		action
# 2:		file
# 3:		identation
#

define pretty-printer-red
  $(call print,blue,[,-n)						; \
  $(call print,red,$(1),-n)						; \
  $(call print,blue,],-n)						; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-green
  $(call print,blue,[,-n)						; \
  $(call print,green,$(1),-n)						; \
  $(call print,blue,],-n)						; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-yellow
  $(call print,blue,[,-n)						; \
  $(call print,yellow,$(1),-n)						; \
  $(call print,blue,],-n)						; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-magenta
  $(call print,blue,[,-n)						; \
  $(call print,magenta,$(1),-n)						; \
  $(call print,blue,],-n)						; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-cyan
  $(call print,blue,[,-n)						; \
  $(call print,cyan,$(1),-n)						; \
  $(call print,blue,],-n)						; \
  $(call print,white,$(3)$(2),)
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
  length=`echo $(3) | $(_WC_) -c`					; \
  if [ $$length -gt 56 ] ; then						\
    name="..."`echo $(3) | $(_TAIL_) -c 53`				; \
  else									\
    name=$(3)								; \
  fi									; \
  $(call pretty-printer-$(1),$(2),$$name,$(4))
endef

#
# call make in each directory of the list
#
# $(1):		the directory list
# $(2):		advanced options
#

define make
  for i in $(1) ; do							\
    $(_CD_) $$i								; \
    $(_MAKE_) $(2)							; \
    return=$$?								; \
    if [ $$return -ne 0 ] ; then					\
      $(call pretty-printer,red,ERROR,$$i,			)	; \
      exit $$return							; \
    fi									; \
    $(_CD_) ..								; \
  done
endef

#
# from c file to object file
#
# $(1):		object file
# $(2):		c file
# $(3):		advanced options
#

define compile-c
  $(call pretty-printer,green,COMPILE-C,$(2),		)		; \
  $(CC) $(CFLAGS) $(3) -c $(2) -o $(1) $(_VERBOSE_)
endef

#
# from S file to object file
#
# $(1):		object file
# $(2):		S file
# $(3):		advanced options
#

define compile-S
  $(call pretty-printer,green,COMPILE-S,$(2),		)		; \
  $(CC) $(CFLAGS) $(3) -c $(2) -o $(1) $(_VERBOSE_)
endef

#
# from asm file to object file
#
# $(1):		object file
# $(2):		asm file
# $(3):		advanced options
#

define compile-asm
  $(call pretty-printer,green,COMPILE-ASM,$(2),		)		; \
  $(_NASM_) $(3) $(2) -o $(1) $(_VERBOSE_)
endef

#
# create a static library from object files
#
# $(1):		static library file name
# $(2):		object files
# $(3):		advanced options
#

define static-library
  $(call pretty-printer,magenta,STATIC-LIBRARY,$(1),	)		; \
  $(_AR_) $(3) $(1) $(2) $(_VERBOSE_)
  $(_RANLIB_) $(3) $(1) $(_VERBOSE_)
endef

#
# create a dynamic library from object files and/or static libraries
# and/or dynamic libraries
#
# $(1):		dynamic library file name
# $(2):		objects files and/or libraries
# $(3):		advanced options
#

define dynamic-library
  $(call pretty-printer,magenta,DYNAMIC-LIBRARY,$(1),	)		; \
  $(_LD_) $(LDFLAGS) --shared $(3) -o $(1) $(2) $(_VERBOSE_)
endef

#
# create an executable file from object file and/or library files
#
# $(1):		executable file name
# $(2):		objects files and/or libraries
# $(3):		advanced options
#

define executable
  $(call pretty-printer,magenta,EXECUTABLE,$(1),		)	; \
  $(_LD_) $(LDFLAGS) $(3) -o $(1) $(2) $(_VERBOSE_)
endef

#
# create an archive file from multiple object files
#
# note that the archive file is also an object file
#
# $(1):		archive file name
# $(2):		objects files and/or libraries
# $(3):		advanced options
#

define archive
  $(call pretty-printer,magenta,ARCHIVE,$(1),		)		; \
  $(_LD_) $(LDFLAGS) -r $(3) -o $(1) $(2) $(_VERBOSE_)
endef

#
# remove the files
#
# $(1):		files
# $(2):		advanced options
#

define remove
  for i in $(1) ; do							\
    if [ -e $$i ] ; then						\
      $(call pretty-printer,red,REMOVE,$$i,		)		; \
    fi									; \
    $(_RM_) $(2) $$i							; \
  done
endef

#
# purge i.e clean the directory from unwanted files
#

define purge
  $(call pretty-printer,red,PURGE,,)					; \
  $(_PURGE_)
endef

#
# generate prototypes from a source file
#
# $(1):		file list
# $(2):		advanced options
#

define prototypes
  for i in $(1) ; do							\
    if [ -e $$i ] ; then						\
      $(call pretty-printer,yellow,PROTOTYPES,$$i,		)	; \
      $(_PROTO_) $(2) $(1)						; \
    fi									; \
  done
endef

#
# genereate dependencies
#
# $(1):		the files for which the dependencies are generated
# $(2):		the output file
# $(3):		advanced options
#

define dependencies
  $(_TOUCH_) $(2)							; \
  for i in $(1) ; do							\
    if [ -e $$i ] ; then						\
      $(call pretty-printer,yellow,DEPENDENCIES,$$i,		)	; \
      $(CC) $(CFLAGS) -M -MG $(3) $$i >> $(2)				; \
    fi									; \
  done
endef

#
# generate a version file
#
# $(1):		the version file to generate
#

define version
  $(call pretty-printer,yellow,VERSION,$(1),		)		; \
  echo -n "" > $(1)							; \
  echo "#include <libc.h>" >> $(1)					; \
  echo "#include <kaneton.h>" >> $(1)					; \
  echo "" >> $(1)							; \
  echo -n "const char version[] = CONF_TITLE\"-\"CONF_VERSION" >> $(1)	; \
  echo "\" $(DATE) $(USER)@$(HOSTNAME)\";" >> $(1)
endef

#
# create a link between two files
#
# $(1):		link created
# $(2):		destination
# $(3):		advances options
#

define link
  $(call pretty-printer,cyan,LINK,$(1),			)		; \
  $(_LN_) $(3) $(2) $(1) $(_VERBOSE_)
endef

#
# create a distribution tarball
#

define dist
  $(call pretty-printer,yellow,DIST,kaneton,			)	; \
  $(_TAR_) -czf /tmp/kaneton.tar.gz .					; \
  $(_MV_) /tmp/kaneton.tar.gz .
endef

# 
# create a dvi file from a tex one
#
# $(1):		the output file name
# $(2):		the logical previous latex file
# $(3):		the orginial latex file
# $(4):		advanced options
#

define dvi
  $(call pretty-printer,yellow,DVI,$(1),			)	; \
  $(_LATEX_) $(4) $(2) -o $(1) $(_VIEW_DEBUG_) $(_VERBOSE_)		; \
  $(_LATEX_) $(4) $(2) -o $(1) $(_VIEW_DEBUG_) $(_VERBOSE_)
endef

# 
# create a ps file from a dvi one
#
# $(1):		the output file name
# $(2):		the logical previous dvi file
# $(3):		the orginial latex file
# $(4):		advanced options
#

define ps
  $(call pretty-printer,yellow,PS,$(1),			)		; \
  $(_DVIPS_) $(4) $(2) -o $(1) $(_VERBOSE_)
endef

# 
# create a pdf file
#
# $(1):		the output file name
# $(2):		the logical previous latex file
# $(3):		the orginial latex file
# $(4):		advanced options
#

define pdf
  $(call pretty-printer,yellow,PDF,$(1),			)	; \
  $(_PDFLATEX_) $(4) $(3) -o $(1) $(_VERBOSE_)				; \
  $(_PDFLATEX_) $(4) $(3) -o $(1) $(_VERBOSE_)
endef
