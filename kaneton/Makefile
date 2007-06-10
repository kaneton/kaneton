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
# updated       julien quintard   [sun jun 10 15:31:35 2007]
#

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

main:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

	$(call env_executable,$(_CORE_),$(CORE_O)			\
					$(VERSION_O)			\
					$(KANETON_O)			\
					$(_KLIBC_A_),			\
                 "--link-script $(_MACHDEP_LINK_DIR_)/core.lks"		\
                 "--no-standard-include"				\
                 "--no-standard-library")

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

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
	  $(call env_launch,$${d}/Makefile,dependencies,)		; \
	done
