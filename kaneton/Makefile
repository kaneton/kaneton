#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/Makefile
#
# created       julien quintard   [sun jun 10 14:54:43 2007]
# updated       julien quintard   [sun jan  9 21:42:48 2011]
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
					machine				\
					library				\
					modules				

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_KANETON_)

$(_KANETON_):		$(_CORE_LO_) $(_MACHINE_LO_)			\
			$(_LIBRARY_LO_) $(_MODULES_LO_)			\
			$(_KERNEL_LAYOUT_)
	$(call env_remove,$(_KANETON_),)

	$(call env_executable,$(_KANETON_),				\
			      $(_CORE_LO_) $(_MACHINE_LO_)		\
			      $(_LIBRARY_LO_) $(_MODULES_LO_),		\
			      $(_KERNEL_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

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
