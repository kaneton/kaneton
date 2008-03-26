#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/lec_l/kanet...profile/host/linux/ia32.mips64/mips64.mk
#
# created       julien quintard   [tue may  8 13:03:34 2007]
# updated       laurent lec   [tue mar 25 13:35:18 2008]
#

#
# ---------- information ------------------------------------------------------
#
# this file implements the remaining functions of the kaneton make interface.
#
# indeed the major generic part of the interface is already provided by the
# host profile.
#

include ../linux.mk

#
# create an executable file from object file and/or library files
#
# $(1):		executable file name
# $(2):		objects files and/or libraries
# $(3):		layout file
# $(4):		options
#

define env_executable
  executable_options=""							&& \
  if [ -n $(3) ] ; then							\
    executable_options="$${executable_options} $(3)"			; \
  fi									&& \
  if [ -n "$(4)" ] ; then						\
    if [ $$(( $(4) & $(ENV_OPTION_NO_STANDARD) )) -ne 0 ] ; then	\
      executable_options="$${executable_options}"	; \
    fi									; \
  fi									&& \
  $(call env_display,magenta,EXECUTABLE,$(1),		,)		&& \
  $(_LD_) $(_LD_FLAGS_) $${executable_options} $(2) -o $(1)
endef

#
# from c file to object file
#
# $(1):		object file
# $(2):		c file
# $(3):		options
#

override define env_compile-c
  compile_c_options=""							&& \
  $(call env_display,green,COMPILE-C,$(2),		,)		&& \
  $(_CC_) $(_CC_FLAGS_) $${compile_c_options} -c $(2) -o $(1)
endef


