##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/machines/gnu-unix/machine.mk
##
## created       julien quintard   [fri feb 11 02:08:31 2005]
## updated       julien quintard   [fri jun 23 16:07:37 2006]
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
  print_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      "--no-newline")							\
        print_options="$${print_options} -n"				; \
        ;;								\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call print-$(1),$(2),$${print_options})
endef

else						# if not ...

define print
  print_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      "--no-newline")							\
        print_options="$${print_options} -n"				; \
        ;;								\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call print-non-color,$(2),$${print_options})
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
  $(call print,blue,[,"--no-newline")					; \
  $(call print,red,$(1),"--no-newline")					; \
  $(call print,blue,],"--no-newline")					; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-green
  $(call print,blue,[,"--no-newline")					; \
  $(call print,green,$(1),"--no-newline")				; \
  $(call print,blue,],"--no-newline")					; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-yellow
  $(call print,blue,[,"--no-newline")					; \
  $(call print,yellow,$(1),"--no-newline")				; \
  $(call print,blue,],"--no-newline")					; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-magenta
  $(call print,blue,[,"--no-newline")					; \
  $(call print,magenta,$(1),"--no-newline")				; \
  $(call print,blue,],"--no-newline")					; \
  $(call print,white,$(3)$(2),)
endef

define pretty-printer-cyan
  $(call print,blue,[,"--no-newline")					; \
  $(call print,cyan,$(1),"--no-newline")				; \
  $(call print,blue,],"--no-newline")					; \
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
  change_directory_options=""						; \
  for o in $(2); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(_CD_) $${change_directory_options} $(1)
endef

#
# launch a new shell script
#
# $(1):		the shell script to launch
# $(2):		arguments
# $(3):		advanced options
#

define shell-script
  shell_script_options=""						; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(_SHELL_) $${shell_script_options} $(1) $(2)
endef

#
# call make in each directory of the list
#
# the directories in the list must be direct subdirectories
#
# $(1):		the directory list
# $(2):		advanced options
#

define makefile
  makefile_options=""							; \
  for o in $(2); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  for d in $(1) ; do							\
    $(call change-directory,$${d},)					; \
    return=$$?								; \
    if [ $${return} -ne 0 ] ; then					\
      exit 0								; \
    fi									; \
    $(_MAKE_) $${makefile_options} $(_MAKEFLAGS_) $(2)			; \
    return=$$?								; \
    if [ $${return} -ne 0 ] ; then					\
      $(call pretty-printer,red,ERROR,$${d},			)	; \
      exit $${return}							; \
    fi									; \
    $(call change-directory,..,)					; \
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
  preprocess_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,green,PREPROCESS,$(2),		)		; \
  $(_CPP_) -P $(_CPPFLAGS_) $${preprocess_options}			\
           $(2) -o $(1) $(_VERBOSE_)
endef

#
# from c file to object file
#
# $(1):		object file
# $(2):		c file
# $(3):		advanced options
#

define compile-c
  compile_c_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,green,COMPILE-C,$(2),		)		; \
  $(_CC_) $(_CFLAGS_) $${compile_c_options}				\
          -c $(2) -o $(1) $(_VERBOSE_)
endef

#
# from lex file to c file
#
# $(1):		c file
# $(2):		lex file
# $(3):		advanced options
#

define lex-l
  lex_l_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,green,LEX-L,$(2),			)	; \
  $(_LEX_) $(2) > $(1)
endef

#
# from S file to object file
#
# $(1):		object file
# $(2):		S file
# $(3):		advanced options
#

define assemble-S
  assemble_S_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,green,ASSEMBLE-S,$(2),		)		; \
  $(_CC_) $(_CFLAGS_) $${assemble_S_options}				\
          -c $(2) -o $(1) $(_VERBOSE_)
endef

#
# from asm file to object file
#
# $(1):		object file
# $(2):		asm file
# $(3):		advanced options
#

define assemble-asm
  assemble_asm_options=""						; \
  for o in $(3); do							\
    case "$${o}" in							\
      "--output-format"*)						\
	assemble_asm_options="$${assemble_asm_options}			\
                              `$(_ECHO_) $${o}				| \
                                $(_SED_) 's/--output-format/-f/1'`"	; \
        ;;								\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,green,ASSEMBLE-ASM,$(2),		)	; \
  $(_NASM_) $${assemble_asm_options} $(2) -o $(1) $(_VERBOSE_)
endef

#
# create a static library from object files
#
# $(1):		static library file name
# $(2):		object files
# $(3):		advanced options
#

define static-library
  static_library_options=""						; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,magenta,STATIC-LIBRARY,$(1),	)		; \
  $(_AR_) $${static_library_options} $(1) $(2) $(_VERBOSE_)		; \
  $(_RANLIB_) $${static_library_options} $(1) $(_VERBOSE_)
endef

#
# create a dynamic library from object files, static libraries
# and/or dynamic libraries
#
# $(1):		dynamic library file name
# $(2):		objects files and/or libraries
# $(3):		advanced options
#

define dynamic-library
  dynamic_library_options=""						; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,magenta,DYNAMIC-LIBRARY,$(1),	)		; \
  $(_LD_) --shared $(_LDFLAGS_) $${dynamic_library_options}		\
          -o $(1) $(2) $(_VERBOSE_)
endef

#
# create an executable file from object file and/or library files
#
# $(1):		executable file name
# $(2):		objects files and/or libraries
# $(3):		advanced options
#

define executable
  executable_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      "--link-script"*)							\
	executable_options="$${executable_options}			\
                            `$(_ECHO_) $${o}				| \
                            $(_SED_) -e 's/--link-script/-T/1'`"	; \
        ;;								\
      "--no-standard-include")						\
	executable_options="$${executable_options} -nostdinc"		; \
        ;;								\
      "--no-standard-library")						\
	executable_options="$${executable_options} -nostdlib"		; \
        ;;								\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,magenta,EXECUTABLE,$(1),		)	; \
  $(_CC_) $(_CFLAGS_) $(_LDFLAGS_) $${executable_options}		\
          -o $(1) $(2) $(_VERBOSE_)
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
  archive_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,magenta,ARCHIVE,$(1),		)		; \
  $(_LD_) -r $(_LDFLAGS_) $${archive_options}				\
          -o $(1) $(2) $(_VERBOSE_)
endef

#
# remove the files
#
# $(1):		files
# $(2):		advanced options
#

define remove
  remove_options=""							; \
  for o in $(2); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  for f in $(1) ; do							\
    if [ -e $${f} ] ; then						\
      $(call pretty-printer,red,REMOVE,$${f},		)		; \
    fi									; \
    $(_RM_) $${remove_options} $${f} $(_VERBOSE_)			; \
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
  prototypes_options=""							; \
  for o in $(2); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  for f in $(1) ; do							\
    if [ -e $${f} ] ; then						\
      $(call pretty-printer,yellow,PROTOTYPES,$${f},		)	; \
      $(_PROTO_TOOL_) $${prototypes_options} $${f} $(_VERBOSE_)		; \
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
  dependencies_options=""						; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(_TOUCH_) $(2)							; \
  for f in $(1) ; do							\
    if [ -e $${f} ] ; then						\
      $(call pretty-printer,yellow,DEPENDENCIES,$$i,		)	; \
      $(_CC_) $(_CFLAGS_) -M -MG $${dependencies_options} $${f} >> $(2)	; \
    fi									; \
  done
endef

#
# generate a version file
#
# $(1):		the version file to generate
# $(2):		the user name
# $(3):		the host name
# $(4):		the date
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
  link_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,cyan,LINK,$(1),			)		; \
  $(_LN_) $${link_options} $(2) $(1) $(_VERBOSE_)
endef

#
# compile a tex document
#
# $(1):		the file name without extension
# $(2):		advanced options
#

define compile-tex
  compile_tex_options=""						; \
  for o in $(2); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call pretty-printer,green,COMPILE-TEX,$(1),		)		; \
  $(_PDFLATEX_) $${compile_tex_options} $(1).tex			\
                -o $(1).pdf $(_VERBOSE_)				; \
  $(_PDFLATEX_) $${compile_tex_options} $(1).tex			\
                -o $(1).pdf $(_VERBOSE_)
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
  $(_ECHO_) '\def\kaneton-latex{subject}' > $(_KANETON_LATEX_)		; \
  $(call compile-tex,$(1),$(2))
endef

#
# build a correction
#
# $(1):		the file name without extension
# $(2):		advanced options
#

define correction
  $(_ECHO_) '\def\kaneton-latex{correction}' > $(_KANETON_LATEX_)	; \
  $(call compile-tex,$(1),$(2))
endef

#
# launch a document viewer
#
# $(1):		the file name without extension
#

define view
  $(call pretty-printer,yellow,VIEW,$(1),			)	; \
  $(_VIEWER_) $(1).pdf $(_VERBOSE_)
endef

#
# returns a file contents
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
