#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ofile/environment/behaviour/behaviour.mk
#
# created       julien quintard   [tue may  8 13:03:34 2007]
# updated       julien quintard   [thu may 10 13:35:28 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this file contains generic stuff about the implementation of the kaneton
# make interface. there are few functions here because make was not designed
# in a portable way.
#

#
# ---------- directives -------------------------------------------------------
#

.SILENT:

#
# ---------- functions --------------------------------------------------------
#

#
# display functions
#
# 1:		action
# 2:		file
# 3:		identation
# 4:		options
#

define display-red
  $(call print,[,blue,$(OPTION_NO_NEWLINE))				; \
  $(call print,$(1),red,$(OPTION_NO_NEWLINE))				; \
  $(call print,],blue,$(OPTION_NO_NEWLINE))				; \
  $(call print,$(3)$(2),,)
endef

define display-green
  $(call print,[,blue,$(OPTION_NO_NEWLINE))				; \
  $(call print,$(1),green,$(OPTION_NO_NEWLINE))				; \
  $(call print,],blue,$(OPTION_NO_NEWLINE))				; \
  $(call print,$(3)$(2),,)
endef

define display-yellow
  $(call print,[,blue,$(OPTION_NO_NEWLINE))				; \
  $(call print,$(1),yellow,$(OPTION_NO_NEWLINE))			; \
  $(call print,],blue,$(OPTION_NO_NEWLINE))				; \
  $(call print,$(3)$(2),,)
endef

define display-magenta
  $(call print,[,blue,$(OPTION_NO_NEWLINE))				; \
  $(call print,$(1),magenta,$(OPTION_NO_NEWLINE))			; \
  $(call print,],blue,$(OPTION_NO_NEWLINE))				; \
  $(call print,$(3)$(2),,)
endef

define display-cyan
  $(call print,[,blue,$(OPTION_NO_NEWLINE))				; \
  $(call print,$(1),cyan,$(OPTION_NO_NEWLINE))				; \
  $(call print,],blue,$(OPTION_NO_NEWLINE))				; \
  $(call print,$(3)$(2),,)
endef

#
# display wrapper
#
# $(1):		color
# $(2):		action
# $(3):		file
# $(4):		indentation
# $(5):		options
#

define display
  $(call display-$(1),$(2),$(3),$(4),$(5))
endef

#
# generate prototypes from a source file
#
# $(1):		file list
# $(2):		options
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
      $(call display,yellow,PROTOTYPES,$${f},		)		; \
      $(call launch,$(_MKP_TOOL_),					\
        $${prototypes_options} $${f},)					; \
    fi									; \
  done
endef

#
# build a paper
#
# $(1):		the file name without extension
# $(2):		options
#

define paper
  $(call compile-tex,$(1),$(2))
endef

#
# build a lecture
#
# $(1):		the file name without extension
# $(2):		options
#

define lecture
  $(call compile-tex,$(1),$(2))
endef
