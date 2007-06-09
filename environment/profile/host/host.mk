#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/environment/profile/host/host.mk
#
# created       julien quintard   [tue may  8 13:03:34 2007]
# updated       julien quintard   [sun jun 10 00:36:35 2007]
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
# ---------- options ----------------------------------------------------------
#

ENV_HEADER_NONE = 0
ENV_HEADER_OK = 1
ENV_HEADER_ERROR = 2
ENV_HEADER_INTERACTIVE = 4

ENV_COLOR_NONE = 0
ENV_COLOR_RED = 1
ENV_COLOR_GREEN = 2
ENV_COLOR_YELLOW = 3
ENV_COLOR_BLUE = 4
ENV_COLOR_WHITE = 5

ENV_OPTION_NONE = 0
ENV_OPTION_NO_NEWLINE = 1
ENV_OPTION_FLICKERING = 2

ENV_OUTPUT_OBJECT = 1
ENV_OUTPUT_BINARY = 2

ENV_OPTION_NO_STANDARD = 1

ENV_OPTION_PRIVATE = 1

#
# ---------- traps ------------------------------------------------------------
#

%.o:			%.asm
	$(call env_assemble-asm,$@,$<,$(ENV_OUTPUT_OBJECT))

%.o:			%.S
	$(call env_assemble-S,$@,$<,)

%.o:			%.c
	$(call env_compile-c,$@,$<,)

%.c:			%.l
	$(call env_lex-l,$@,$<,)

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

define env_display-red
  $(call env_print,[,blue,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,$(1),red,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,],blue,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,$(3)$(2),,)
endef

define env_display-green
  $(call env_print,[,blue,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,$(1),green,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,],blue,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,$(3)$(2),,)
endef

define env_display-yellow
  $(call env_print,[,blue,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,$(1),yellow,$(ENV_OPTION_NO_NEWLINE))		; \
  $(call env_print,],blue,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,$(3)$(2),,)
endef

define env_display-magenta
  $(call env_print,[,blue,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,$(1),magenta,$(ENV_OPTION_NO_NEWLINE))		; \
  $(call env_print,],blue,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,$(3)$(2),,)
endef

define env_display-cyan
  $(call env_print,[,blue,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,$(1),cyan,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,],blue,$(ENV_OPTION_NO_NEWLINE))			; \
  $(call env_print,$(3)$(2),,)
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

define env_display
  $(call env_display-$(1),$(2),$(3),$(4),$(5))
endef
