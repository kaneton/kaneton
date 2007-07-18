#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/crypt/kaneton/kaneton/Makefile
#
# created       julien quintard   [sun jun 10 14:54:43 2007]
# updated       matthieu bucchianeri   [tue jul 17 17:59:36 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton

#
# ---------- dependencies -----------------------------------------------------
#

include			../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		core				\
					machine

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_KANETON_)

$(_KANETON_):		$(_CORE_LO_) $(_MACHINE_LO_) libs
	$(call env_remove,$(_KANETON_),)

	$(call env_executable,$(_KANETON_),				\
			      $(_CORE_LO_) $(_MACHINE_LO_)		\
			      $(_TEST_LO_),				\
			      $(_KANETON_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))
libs:
	  $(call env_launch,${_TEST_DIR_}/Makefile,,)

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
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

endif
