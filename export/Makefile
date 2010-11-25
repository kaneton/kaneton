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
# updated       julien quintard   [tue nov 23 22:28:16 2010]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main export- export clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		modules

#
# ---------- rules ------------------------------------------------------------
#

main:

export- export:
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
