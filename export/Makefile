#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/export/Makefile
#
# created       julien quintard   [sun jun 10 14:53:00 2007]
# updated       julien quintard   [wed oct 20 13:00:10 2010]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main export clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		modules

#
# ---------- rules ------------------------------------------------------------
#

main:

export:
	$(call env_launch,$(_EXPORT_SCRIPT_),$*,)

export-%:
	$(call env_launch,$(_EXPORT_SCRIPT_),$*,)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_purge)

prototypes:

headers:
