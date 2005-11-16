##
## copyright quintard julien
## 
## kaneton
## 
## machine.mk
## 
## path          /home/mycure/kaneton/export
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:08:31 2005   mycure
## last update   Mon Nov 14 20:26:17 2005   mycure
##

#
# ---------- information ------------------------------------------------------
#
# this file contains everything needed by the kaneton make system.
#
# the main part of this file is the functions part which is composed
# of the functions used to compile everything on this operating system.
#

#
# ---------- directives -------------------------------------------------------
#

.SILENT:

#
# ---------- traps ------------------------------------------------------------
#

%.o:			%.asm
	$(call assemble-asm,$@,$<,-f elf)

%.o:			%.S
	$(call assemble-S,$@,$<,)

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
# print functions wrapper
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
  $(call pretty-printer-$(1),$(2),$(3),$(4))
endef

#
# change the current working directory
#
# $(1):		directory
# $(2):		advanced options
#

define change-directory
  $(_CD_) $(2) $(1)
endef

#
# launch a new program, script, makefile etc..
#
# $(1):		the file to launch
# $(2):		arguments
# $(3):		advanced options
#

define launch
  case "$(1)" in							\
    Makefile)								\
      $(_MAKE_) $(_MAKEFLAGS_) $(3) $(1) $(2)				; \
      ;;								\
    *.sh)								\
      $(_SHELL_) $(3) $(1) $(2)						; \
      ;;								\
    *)									\
      ;;								\
  esac
endef

#
# call make in each directory of the list
#
# $(1):		the directory list
# $(2):		advanced options
#

define makefile
  for i in $(1) ; do							\
    $(_CD_) $$i								; \
    $(_MAKE_) $(_MAKEFLAGS_) $(2)					; \
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
  $(_CC_) $(_CFLAGS_) $(3) -c $(2) -o $(1) $(_VERBOSE_)
endef

#
# from S file to object file
#
# $(1):		object file
# $(2):		S file
# $(3):		advanced options
#

define assemble-S
  $(call pretty-printer,green,ASSEMBLE-S,$(2),		)		; \
  $(_CC_) $(_CFLAGS_) $(3) -c $(2) -o $(1) $(_VERBOSE_)
endef

#
# from asm file to object file
#
# $(1):		object file
# $(2):		asm file
# $(3):		advanced options
#

define assemble-asm
  $(call pretty-printer,green,ASSEMBLE-ASM,$(2),		)	; \
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
  $(_LD_) $(_LDFLAGS_) --shared $(3) -o $(1) $(2) $(_VERBOSE_)
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
  $(_LD_) $(_LDFLAGS_) $(3) -o $(1) $(2) $(_VERBOSE_)
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
  $(_LD_) $(_LDFLAGS_) -r $(3) -o $(1) $(2) $(_VERBOSE_)
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
    $(_RM_) $(2) $$i $(_VERBOSE_)					; \
  done
endef

#
# purge i.e clean the directory from unwanted files
#

define purge
  $(call pretty-printer,red,PURGE,,)					; \
  $(_PURGE_) $(_VERBOSE_)
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
      $(_PROTO_TOOL_) $(2) $(1)	$(_VERBOSE_)				; \
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
      $(_CC_) $(_CFLAGS_) -M -MG $(3) $$i >> $(2)			; \
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
  echo "#include <klibc.h>" >> $(1)					; \
  echo "#include <kaneton.h>" >> $(1)					; \
  echo "" >> $(1)							; \
  echo -n "const char version[] = CONF_TITLE\"-\"CONF_VERSION" >> $(1)	; \
  echo "\" $(4) $(2)@$(3)\";" >> $(1)
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
# compile a tex document
#
# $(1):		the file name without extension
#

define compile-tex
  $(call pretty-printer,green,COMPILE-TEX,$(1),			)	; \
  case $(_FORMAT_) in							\
    dvi)								\
      $(_LATEX_) $(1).tex -o $(1).dvi $(_VERBOSE_)			; \
      $(_LATEX_) $(1).tex -o $(1).dvi $(_VERBOSE_)			; \
      ;;								\
    ps)									\
      $(_LATEX_) $(1).tex -o $(1).dvi $(_VERBOSE_)			; \
      $(_LATEX_) $(1).tex -o $(1).dvi $(_VERBOSE_)			; \
      $(_DVIPS_) $(1).dvi -o $(1).ps $(_VERBOSE_)			; \
      ;;								\
    pdf)								\
      $(_PDFLATEX_) $(1).tex -o $(1).pdf $(_VERBOSE_)			; \
      $(_PDFLATEX_) $(1).tex -o $(1).pdf $(_VERBOSE_)			; \
      ;;								\
  esac
endef

#
# launch a document viewer
#
# $(1):		the file name without extension
#

define view
  $(call pretty-printer,yellow,VIEW,$(1),			)	; \
  case $(_FORMAT_) in							\
    dvi)								\
      $(_VIEWER_DVI_) $(1).dvi $(_VERBOSE_)				; \
      ;;								\
    ps)									\
      $(_VIEWER_PS_) $(1).ps $(_VERBOSE_)				; \
      ;;								\
    pdf)								\
      $(_VIEWER_PDF_) $(1).pdf $(_VERBOSE_)				; \
      ;;								\
  esac
endef

#
# returns the current data
#

define date
  $(_DATE_)
endef
