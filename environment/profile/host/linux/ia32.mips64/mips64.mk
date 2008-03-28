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
# updated       laurent lec   [fri mar 28 16:06:23 2008]
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


ifeq ($(component),kaneton)

main:			dependencies $(_KANETON_)

$(_KANETON_):		$(_CORE_LO_) $(_MACHINE_LO_)
	$(call env_remove,$(_KANETON_),)

	$(call env_executable,$(_KANETON_),				\
			      $(_MACHINE_LO_) $(_LIBC_LO_)		\
			      $(_TEST_LO_) $(_BOOTLOADER_LO_),		\
			      $(_KANETON_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))


#			      $(_CORE_LO_)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(_KANETON_),)

	$(call env_purge,)

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

dependencies:
	$(call env_launch,$(_BOOTLOADER_DIR_)/Makefile,,)
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done


behaviour             :=              specific

endif

