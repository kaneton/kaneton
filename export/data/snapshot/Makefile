#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kanetno
#
# file          /home/mycure/kaneton/export/data/snapshot/Makefile
#
# created       julien quintard   [tue jun 26 11:27:22 2007]
# updated       julien quintard   [sat feb  5 12:11:16 2011]
#

#
# ---------- dependencies -----------------------------------------------------
#

-include		environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.SILENT:

.PHONY:		main initialize clean clear prototypes			\
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

PATHS			=		$(dir $(_COMPONENTS_))

  main:
	for path in $(PATHS) ; do					\
          if [ -f "$${path}/Makefile" ] ; then				\
	    $(call env_launch,$${path}/Makefile,,)			; \
	  fi								\
	done

else

  main									\
  clear									\
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
	cd environment/							&& \
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
	$(call env_launch,$(_CLEAN_SCRIPT_),,)

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		boot environment kaneton	\
					license sample test tool	\
					$(PATHS)

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
	$(call env_print,http://kaneton.opaak.org,,)

	$(call env_print,,,)

endif
