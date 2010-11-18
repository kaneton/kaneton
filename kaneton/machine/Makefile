#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/kaneton/machine/Makefile
#
# created       julien quintard   [sun jun 10 14:54:43 2007]
# updated       julien quintard   [thu nov 18 15:53:28 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/machine

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		$(_ARCHITECTURE_DIR_)		\
					$(_GLUE_DIR_)			\
					$(_PLATFORM_DIR_)		

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_MACHINE_LO_)

$(_MACHINE_LO_):	$(_GLUE_LO_) $(_ARCHITECTURE_LO_) $(_PLATFORM_LO_)
	$(call env_remove,$(_MACHINE_LO_),)

	$(call env_archive,$(_MACHINE_LO_),				\
			   $(_GLUE_LO_)					\
			   $(_ARCHITECTURE_LO_)				\
			   $(_PLATFORM_LO_),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(_MACHINE_LO_),)

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
