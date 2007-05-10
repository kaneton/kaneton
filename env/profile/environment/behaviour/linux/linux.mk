#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ile/environment/behaviour/linux/linux.mk
#
# created       julien quintard   [tue may  8 13:03:34 2007]
# updated       julien quintard   [thu may 10 13:34:19 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this file implements the remaining functions of the kaneton make interface.
#
# indeed the major generic part of the interface is already provided by the
# behaviour profile.
#

#
# ---------- traps ------------------------------------------------------------
#

%.o:			%.asm
	$(call assemble-asm,$@,$<,"--output-format elf")

%.o:			%.S
	$(call assemble-S,$@,$<,)

%.o:			%.c
	$(call compile-c,$@,$<,)

%.c:			%.l
	$(call lex-l,$@,$<,)

#
# ---------- functions --------------------------------------------------------
#

#
# colorize functions
#
# 1:		text
# 2:		options
#

define colorize-black
  '\E[;30m'"\033[1m$(1)\033[0m"
endef

define colorize-red
  '\E[;31m'"\033[1m$(1)\033[0m"
endef

define colorize-green
  '\E[;32m'"\033[1m$(1)\033[0m"
endef

define colorize-yellow
  '\E[;33m'"\033[1m$(1)\033[0m"
endef

define colorize-blue
  '\E[;34m'"\033[1m$(1)\033[0m"
endef

define colorize-magenta
  '\E[;35m'"\033[1m$(1)\033[0m"
endef

define colorize-cyan
  '\E[;36m'"\033[1m$(1)\033[0m"
endef

define colorize-white
  '\E[;37m'"\033[1m$(1)\033[0m"
endef

define colorize-
  $(1)
endef

#
# print functions wrapper
#
# 1:		text
# 2:		color
# 3:		options
#

ifeq ($(_DISPLAY_),$(_DISPLAY_COLORED_))	# if the user wants to display
						# the text with color

define print
  print_options=""							; \
  if [ $(( $(3) & $(OPTION_NO_NEWLINE) )) -ne 0 ] ; then		\
    print_options="$${print_options} -n"				; \
  fi									; \
  $(_ECHO_) $${print_options} $(call colorize-$(2),$(1),)
endef

else						# if not ...

define print
  print_options=""							; \
  if [ $(( $(3) & $(OPTION_NO_NEWLINE) )) -ne 0 ] ; then		\
    print_options="$${print_options} -n"				; \
  fi									; \
  $(_ECHO_) $${print_options} $(1)
endef

endif

#
# change the current working directory
#
# $(1):		directory
# $(2):		options
#

define cd
  cd_options=""								; \
  $(_CD_) $${cd_options} $(1)
endef

#
# launch a new program/script/make etc..
#
# $(1):		file
# $(2):		arguments
# $(3):		options
#

define launch
  launch_options=""							; \
  case "$(1)" in							\
    *.sh)								\
      $(_SHELL_) $${launch_options} $(1) $(2)				; \
      ;;								\
    *.py)								\
      pp=$(PYTHONPATH)							; \
      export PYTHONPATH="$(PWD):"$(PYTHONPATH)				; \
      $(_PYTHON_) $${launch_options} $(1) $(2)				; \
      export PYTHONPATH=$(pp)						; \
      ;;								\
    *.pl)								\
      $(_PERL_) $${launch_options} $(1) $(2)				; \
      ;;								\
    Makefile)								\
      $(_MAKE_) $${launch_options} -f $(1) $(_MAKE_FLAGS_) ${2}		; \
      ;;								\
  esac
endef

#
# from c file to preprocessed c file
#
# $(1):		preprocessed file
# $(2):		c file
# $(3):		options
#

define preprocess
  preprocess_options=""							; \
  $(call display,green,PREPROCESS,$(2),		)			; \
  $(_CPP_) -P $(_CPP_FLAGS_) $${preprocess_options} $(2) -o $(1)
endef

#
# from c file to object file
#
# $(1):		object file
# $(2):		c file
# $(3):		options
#

define compile-c
  compile_c_options=""							; \
  $(call display,green,COMPILE-C,$(2),		)			; \
  $(_CC_) $(_CC_FLAGS_) $${compile_c_options} -c $(2) -o $(1)
endef

#
# from lex file to c file
#
# $(1):		c file
# $(2):		lex file
# $(3):		options
#

define lex-l
  lex_l_options=""							; \
  $(call display,green,LEX-L,$(2),			)		; \
  $(_LEX_) $${lex_l_options} $(2) > $(1)
endef

#
# from S file to object file
#
# $(1):		object file
# $(2):		S file
# $(3):		options
#

define assemble-S
  assemble_S_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call display,green,ASSEMBLE-S,$(2),		)			; \
  $(_CC_) $(_CC_FLAGS_) $${assemble_S_options}				\
          -c $(2) -o $(1)
endef

#
# from asm file to object file
#
# $(1):		object file
# $(2):		asm file
# $(3):		options
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
  $(call display,green,ASSEMBLE-ASM,$(2),		)		; \
  $(_NASM_) $${assemble_asm_options} $(2) -o $(1)
endef

#
# create a static library from object files
#
# $(1):		static library file name
# $(2):		object files
# $(3):		options
#

define static-library
  static_library_options=""						; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call display,magenta,STATIC-LIBRARY,$(1),	)			; \
  $(_AR_) $${static_library_options} $(1) $(2)				; \
  $(_RANLIB_) $${static_library_options} $(1)
endef

#
# create a dynamic library from object files, static libraries
# and/or dynamic libraries
#
# $(1):		dynamic library file name
# $(2):		objects files and/or libraries
# $(3):		options
#

define dynamic-library
  dynamic_library_options=""						; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call display,magenta,DYNAMIC-LIBRARY,$(1),	)			; \
  $(_LD_) --shared $(_LD_FLAGS_) $${dynamic_library_options}		\
          -o $(1) $(2)
endef

#
# create an executable file from object file and/or library files
#
# $(1):		executable file name
# $(2):		objects files and/or libraries
# $(3):		options
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
  $(call display,magenta,EXECUTABLE,$(1),		)		; \
  $(_CC_) $(_CC_FLAGS_) $(_LD_FLAGS_) $${executable_options}		\
          -o $(1) $(2)
endef

#
# create an archive file from multiple object files
#
# note that the archive file is also an object file
#
# $(1):		archive file name
# $(2):		objects files and/or libraries
# $(3):		options
#

define archive
  archive_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call display,magenta,ARCHIVE,$(1),		)			; \
  $(_LD_) -r $(_LD_FLAGS_) $${archive_options}				\
          -o $(1) $(2)
endef

#
# remove the files
#
# $(1):		files
# $(2):		options
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
      $(call display,red,REMOVE,$${f},		)			; \
    fi									; \
    $(_RM_) $${remove_options} $${f}					; \
  done
endef

#
# purge i.e clean the directory from unwanted files
#

define purge
  $(call display,red,PURGE,,)						; \
  $(_PURGE_)
endef

#
# genereate dependencies
#
# $(1):		the files for which the dependencies are generated
# $(2):		the output file
# $(3):		options
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
      $(call display,yellow,DEPENDENCIES,$$i,		)		; \
      $(_CC_) $(_CC_FLAGS_) -M -MG $${dependencies_options}		\
        $${f} >> $(2)							; \
    fi									; \
  done
endef

#
# generate a version file
#
# $(1):		the version file to generate
#

define version
  $(call display,yellow,VERSION,$(1),		)			; \
  $(_ECHO_) -n "" > $(1)						; \
  $(_ECHO_) "#include <klibc.h>" >> $(1)				; \
  $(_ECHO_) "#include <kaneton.h>" >> $(1)				; \
  $(_ECHO_) "" >> $(1)							; \
  $(_ECHO_) -n "const char version[] = \"$(_TITLE_)-$(_VERSION_)" >> $(1) ; \
  $(_ECHO_) " "$(shell $(_DATE_))" $(USER)@$(HOSTNAME)\";" >> $(1)
endef

#
# create a link between two files
#
# $(1):		link created
# $(2):		destination
# $(3):		options
#

define link
  link_options=""							; \
  for o in $(3); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call display,cyan,LINK,$(1),			)		; \
  $(_LN_) $${link_options} $(2) $(1)
endef

#
# compile a tex document
#
# $(1):		the file name without extension
# $(2):		options
#

define compile-tex
  compile_tex_options=""						; \
  for o in $(2); do							\
    case "$${o}" in							\
      *)								\
        ;;								\
    esac								; \
  done									; \
  $(call display,green,COMPILE-TEX,$(1),		)		; \
  $(_PDFLATEX_) $${compile_tex_options} $(1).tex			\
                -o $(1).pdf				; \
  $(_PDFLATEX_) $${compile_tex_options} $(1).tex			\
                -o $(1).pdf
endef

#
# build a subject
#
# $(1):		the file name without extension
# $(2):		options
#

define subject
  $(_ECHO_) '\def\kaneton-latex{subject}' > $(_DEPENDENCY_TEX_)		; \
  $(call compile-tex,$(1),$(2))
endef

#
# build a correction
#
# $(1):		the file name without extension
# $(2):		options
#

define correction
  $(_ECHO_) '\def\kaneton-latex{correction}' > $(_DEPENDENCY_TEX_)	; \
  $(call compile-tex,$(1),$(2))
endef

#
# launch a document viewer
#
# $(1):		the file name without extension
# $(2):		options
#

define view
  $(call display,yellow,VIEW,$(1),			)		; \
  $(_XPDF_) $(1).pdf
endef

#
# returns a file contents
#
# $(1):		the file
# $(2):		options
#

define contents
  $(_CAT_) $(2) $(1)
endef
