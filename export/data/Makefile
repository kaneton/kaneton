#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kanetno
#
# file          /home/mycure/kaneton/export/data/Makefile
#
# created       julien quintard   [tue jun 26 11:27:22 2007]
# updated       julien quintard   [tue may  5 12:21:08 2009]
#

#
# ---------- dependencies -----------------------------------------------------
#

-include		environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.SILENT:

.PHONY:		main initialize clean kaneton clear prototypes		\
		headers build install info

#
# ---------- variables --------------------------------------------------------
#

_PYTHON_		?=		$(KANETON_PYTHON)
_MAKE_			?=		$(MAKE)

#
# ---------- default rule -----------------------------------------------------
#

ifeq ($(_SIGNATURE_),kaneton)

PATHS			=		$(dir $(_INPUTS_))

  main:
	for path in $(PATHS) ; do					\
	  $(call env_launch,$${path}/Makefile,,)			; \
	done

else

  main									\
  kaneton clear								\
  prototypes								\
  headers								\
  build install								\
  info									\
  clean:	initialize
	$(_MAKE_) -f Makefile $@

endif

#
# ---------- environment ------------------------------------------------------
#

initialize:
	cd environment							&& \
	$(_PYTHON_) initialize.py					&& \
	cd ..

#
# ---------- conditional ------------------------------------------------------
#

ifeq ($(_SIGNATURE_),kaneton)

#
# ---------- environment ------------------------------------------------------
#

clean:
	$(call env_launch,$(_ENVIRONMENT_DIR_)/clean.py,,)

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		environment \
					$(PATHS)

#
# ---------- kaneton ----------------------------------------------------------
#

kaneton:
	$(call env_launch,$(_KANETON_DIR_)/Makefile,,)

#
# ---------- clear ------------------------------------------------------------
#

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_purge,)

#
# ---------- prototypes -------------------------------------------------------
#

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

#
# ---------- headers ----------------------------------------------------------
#

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

#
# ---------- boot -------------------------------------------------------------
#

build:
	$(call env_launch,$(_MBL_SCRIPT_),build,)

install:		main
	$(call env_launch,$(_MBL_SCRIPT_),install,)

#
# ---------- information ------------------------------------------------------
#

info:
	$(call env_print,,,)

	$(call env_print,"--- ",blue,$(ENV_OPTION_NO_NEWLINE))
	$(call env_print,http://www.kaneton.org,,)

	$(call env_print,,,)

endif
