#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/environment/profile/host/host.mk
#
# created       julien quintard   [tue may  8 13:03:34 2007]
# updated       julien quintard   [mon nov 29 10:18:58 2010]
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

ENV_OPTION_NO_STANDARD = 1
ENV_OPTION_STANDARD = 2

ENV_OPTION_PRIVATE = 1

#
# ---------- default rule -----------------------------------------------------
#
# this rule is invoked when a Makefile is called without specifying a
# rule name.
#
# this rule is intended to check whether the configured environment files
# are up-to-date. if not, this rule re-builds the configured environment files.
#

ENV_DEPENDENCIES := $(wildcard						\
		$(_PROFILE_DIR_)/*.conf					\
		$(_PROFILE_DIR_)/host/*.conf				\
		$(_PROFILE_DIR_)/host/*.mk				\
		$(_PROFILE_DIR_)/host/*.py				\
		$(_PROFILE_HOST_DIR_)/*.conf				\
		$(_PROFILE_HOST_DIR_)/*.mk				\
		$(_PROFILE_HOST_DIR_)/*.py				\
		$(_PROFILE_DIR_)/kaneton/*.conf				\
		$(_PROFILE_DIR_)/kaneton/*.py				\
		$(_PROFILE_DIR_)/kaneton/*.mk				\
		$(_PROFILE_LIBRARY_DIR_)/*.conf				\
		$(_PROFILE_LIBRARY_DIR_)/*.mk				\
		$(_PROFILE_LIBRARY_DIR_)/*.py				\
		$(_PROFILE_MODULES_DIR_)/*.conf				\
		$(_PROFILE_MODULES_DIR_)/*.mk				\
		$(_PROFILE_MODULES_DIR_)/*.py				\
		$(_PROFILE_MACHINE_DIR_)/*.conf				\
		$(_PROFILE_MACHINE_DIR_)/*.mk				\
		$(_PROFILE_MACHINE_DIR_)/*.py				\
		$(_PROFILE_PLATFORM_DIR_)/*.conf			\
		$(_PROFILE_PLATFORM_DIR_)/*.mk				\
		$(_PROFILE_PLATFORM_DIR_)/*.py				\
		$(_PROFILE_ARCHITECTURE_DIR_)/*.conf			\
		$(_PROFILE_ARCHITECTURE_DIR_)/*.mk			\
		$(_PROFILE_ARCHITECTURE_DIR_)/*.py			\
		$(_PROFILE_GLUE_DIR_)/*.conf				\
		$(_PROFILE_GLUE_DIR_)/*.mk				\
		$(_PROFILE_GLUE_DIR_)/*.py				\
		$(_PROFILE_CORE_DIR_)/*.conf				\
		$(_PROFILE_CORE_DIR_)/*.mk				\
		$(_PROFILE_CORE_DIR_)/*.py				\
		$(_PROFILE_DIR_)/user/*.conf				\
		$(_PROFILE_DIR_)/user/*.mk				\
		$(_PROFILE_DIR_)/user/*.py				\
		$(_PROFILE_USER_DIR_)/*.conf				\
		$(_PROFILE_USER_DIR_)/*.mk				\
		$(_PROFILE_USER_DIR_)/*.py)

_:		$(_ENV_MK_) $(_ENV_PY_)
	$(call env_launch,Makefile,main,)

$(_ENV_MK_) $(_ENV_PY):							\
		$(ENV_DEPENDENCIES)
	$(call env_launch,$(_ENVIRONMENT_DIR_)/Makefile,initialize,)

#
# ---------- traps ------------------------------------------------------------
#

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
  $(call env_print,[,blue,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,$(1),red,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,],blue,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,$(3)$(2),,)
endef

define env_display-green
  $(call env_print,[,blue,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,$(1),green,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,],blue,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,$(3)$(2),,)
endef

define env_display-yellow
  $(call env_print,[,blue,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,$(1),yellow,$(ENV_OPTION_NO_NEWLINE))		&& \
  $(call env_print,],blue,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,$(3)$(2),,)
endef

define env_display-magenta
  $(call env_print,[,blue,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,$(1),magenta,$(ENV_OPTION_NO_NEWLINE))		&& \
  $(call env_print,],blue,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,$(3)$(2),,)
endef

define env_display-cyan
  $(call env_print,[,blue,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,$(1),cyan,$(ENV_OPTION_NO_NEWLINE))			&& \
  $(call env_print,],blue,$(ENV_OPTION_NO_NEWLINE))			&& \
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

#
# ---------- information ------------------------------------------------------
#

directory		:=		$(CURDIR)

#
# ---------- component-based behaviour ----------------------------------------
#

#
# behaviour
#

behaviour		:=		default

#
# components
#

ifeq ($(call findstring,$(component),$(components)),)
  components		+=		$(component)
endif

export components

#
# ---------- cc flags ---------------------------------------------------------
#

_CC_FLAGS_		+=		$(foreach m,$(_MODULES_),-DMODULE_$(m))
