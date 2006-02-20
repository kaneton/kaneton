##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/machines/macos-ppc.ia32/machine.mk
##
## created       julien quintard   [fri feb 11 02:08:31 2005]
## updated       julien quintard   [sat feb 18 19:05:05 2006]
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
	$(call assemble-asm,$@,$<,"--format elf")

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
  options='';								\
  for opt in $(3); do							\
    case "$$opt" in							\
      "--no-newline")							\
        options="$$options -n"						\
        ;;								\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call print-$(1),$(2),$$options)
endef

else						# if not ...

define print
  options='';								\
  for opt in $(3); do							\
    case "$$opt" in							\
      "--no-newline")							\
        options="$$options -n"						\
        ;;								\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call print-non-color,$(2),$$options)
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
  $(call print,blue,[,--no-newline)					; \
  $(call print,red,$(1),--no-newline)					; \
  $(call print,blue,],--no-newline)					; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-green
  $(call print,blue,[,--no-newline)					; \
  $(call print,green,$(1),--no-newline)					; \
  $(call print,blue,],--no-newline)					; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-yellow
  $(call print,blue,[,--no-newline)					; \
  $(call print,yellow,$(1),--no-newline)				; \
  $(call print,blue,],--no-newline)					; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-magenta
  $(call print,blue,[,--no-newline)					; \
  $(call print,magenta,$(1),--no-newline)				; \
  $(call print,blue,],--no-newline)					; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-cyan
  $(call print,blue,[,--no-newline)					; \
  $(call print,cyan,$(1),--no-newline)					; \
  $(call print,blue,],--no-newline)					; \
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
  options='';								\
  for opt in $(2); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(_CD_) $$options $(1)
endef

#
# launch a new program, script etc..
#
# $(1):		the file to launch
# $(2):		arguments
# $(3):		advanced options
#

define launch
  $(_SHELL_) $(3) $(1) $(2)
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
    return=$$?								; \
    if [ $$return -ne 0 ] ; then					\
      exit 0								; \
    fi									; \
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
# from c file to preprocessed c file
#
# $(1):		preprocessed file
# $(2):		c file
# $(3):		advanced options
#

define preprocess
  options_pp='';							\
  for opt in $(3); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call pretty-printer,green,PREPROCESS,$(2),		)		; \
  $(_CPP_) -P $(_CPPFLAGS_) $$options_pp $(2) -o $(1) $(_VERBOSE_)
endef

#
# from c file to object file
#
# $(1):		object file
# $(2):		c file
# $(3):		advanced options
#

define compile-c
  options_cc='';							\
  for opt in $(3); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call pretty-printer,green,COMPILE-C,$(2),		)		; \
  $(_CC_) $(_CFLAGS_) $$options_cc -c $(2) -o $(1) $(_VERBOSE_)
endef

#
# from S file to object file
#
# $(1):		object file
# $(2):		S file
# $(3):		advanced options
#

define assemble-S
  options_as='';							\
  for opt in $(3); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call pretty-printer,green,ASSEMBLE-S,$(2),		)		; \
  $(_CC_) $(_CFLAGS_) $$options_as -c $(2) -o $(1) $(_VERBOSE_)
endef

#
# from asm file to object file
#
# $(1):		object file
# $(2):		asm file
# $(3):		advanced options
#

define assemble-asm
  options_aa='';							\
  for opt in $(3); do							\
    case "$$opt" in							\
      "--format"*)							\
	options_aa="$$options_aa `$(_ECHO_) $$opt | $(_SED_) -e		\
			's/--format/-f/1'`"				\
        ;;								\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call pretty-printer,green,ASSEMBLE-ASM,$(2),		)	; \
  $(_NASM_) $$options_aa $(2) -o $(1) $(_VERBOSE_)
endef

#
# create a static library from object files
#
# $(1):		static library file name
# $(2):		object files
# $(3):		advanced options
#

define static-library
  options_sl='';							\
  for opt in $(3); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call pretty-printer,magenta,STATIC-LIBRARY,$(1),	)		; \
  $(_AR_) $$options_sl $(1) $(2) $(_VERBOSE_)				; \
  $(_RANLIB_) $$options_sl $(1) $(_VERBOSE_)
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
  options_dl='';							\
  for opt in $(3); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call pretty-printer,magenta,DYNAMIC-LIBRARY,$(1),	)		; \
  $(_LD_) $(_LDFLAGS_) --shared $$options_dl -o $(1) $(2) $(_VERBOSE_)
endef

#
# create an executable file from object file and/or library files
#
# $(1):		executable file name
# $(2):		objects files and/or libraries
# $(3):		advanced options
#

define executable
  options_e='';								\
  for opt in $(3); do							\
    case "$$opt" in							\
      "--lds"*)								\
	options_e="$$options_e `$(_ECHO_) $$opt | $(_SED_) -e		\
		's/--lds/-T/1'`"					\
        ;;								\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call pretty-printer,magenta,EXECUTABLE,$(1),		)	; \
  $(_LD_) $(_LDFLAGS_) $$options_e -o $(1) $(2) $(_VERBOSE_)
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
  options_ar='';							\
  for opt in $(3); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call pretty-printer,magenta,ARCHIVE,$(1),		)		; \
  $(_LD_) $(_LDFLAGS_) -r $$options_ar -o $(1) $(2) $(_VERBOSE_)
endef

#
# remove the files
#
# $(1):		files
# $(2):		advanced options
#

define remove
  options_rm='';							\
  for opt in $(2); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  for i in $(1) ; do							\
    if [ -e $$i ] ; then						\
      $(call pretty-printer,red,REMOVE,$$i,		)		; \
    fi									; \
    $(_RM_) $$options_rm $$i $(_VERBOSE_)				; \
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
  options_proto='';							\
  for opt in $(2); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  for i in $(1) ; do							\
    if [ -e $$i ] ; then						\
      $(call pretty-printer,yellow,PROTOTYPES,$$i,		)	; \
      $(_PROTO_TOOL_) $$options_proto $(1)	$(_VERBOSE_)		; \
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
  options_dep='';							\
  for opt in $(3); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(_TOUCH_) $(2)							; \
  for i in $(1) ; do							\
    if [ -e $$i ] ; then						\
      $(call pretty-printer,yellow,DEPENDENCIES,$$i,		)	; \
      $(_CC_) $(_CFLAGS_) -M -MG $$options_dep $$i >> $(2)		; \
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
  echo -n "const char version[] = \"$(_TITLE_)-$(_VERSION_)" >> $(1)	; \
  echo " $(4) $(2)@$(3)\";" >> $(1)
endef

#
# create a link between two files
#
# $(1):		link created
# $(2):		destination
# $(3):		advanced options
#

define link
 options_ln='';								\
  for opt in $(3); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call pretty-printer,cyan,LINK,$(1),			)		; \
  $(_LN_) $$options_ln $(2) $(1) $(_VERBOSE_)
endef

#
# compile a tex document
#
# $(1):		the file name without extension
# $(2):		advanced options
#

define compile-tex
  options_tex='';							\
  for opt in $(2); do							\
    case "$$opt" in							\
      *)								\
        ;;								\
    esac;								\
  done;									\
  $(call pretty-printer,green,COMPILE-TEX,$(1),		)		; \
  $(_PDFLATEX_) $$options_tex $(1).tex -o $(1).pdf $(_VERBOSE_)		; \
  $(_PDFLATEX_) $$options_tex $(1).tex -o $(1).pdf $(_VERBOSE_)
endef

#
# build a paper
#
# $(1):		the file name without extension
# $(2):		advanced options
#

define paper
  $(call compile-tex,$(1),$(2))
endef

#
# build a lecture
#
# $(1):		the file name without extension
# $(2):		advanced options
#

define lecture
  $(call compile-tex,$(1),$(2))
endef

#
# build a subject
#
# $(1):		the file name without extension
# $(2):		advanced options
#

define subject
  $(_ECHO_) '\def\cmode{subject}' > $(_LATEX_MODE_)			; \
  $(call compile-tex,$(1),$(2))
endef

#
# build a correction
#
# $(1):		the file name without extension
# $(2):		advanced options
#

define correction
  $(_ECHO_) '\def\cmode{correction}' > $(_LATEX_MODE_)			; \
  $(call compile-tex,$(1),$(2))
endef

#
# launch a document viewer
#
# $(1):		the file name without extension
#

define view
  $(call pretty-printer,yellow,VIEW,$(1),			)	; \
  $(_PDFVIEWER_) $(1).pdf $(_VERBOSE_)
endef

#
# gets a file contents
#
# $(1):		the file
# $(2):		advanced options
#

define contents
  $(shell $(_CAT_) $(2) $(1))
endef

#
# returns the current data
#

define date
  $(shell $(_DATE_))
endef
