#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.NEW/kaneton/modules/report/Makefile
#
# created       julien quintard   [wed apr 15 07:00:50 2009]
# updated       julien quintard   [tue nov 23 10:34:52 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/report

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

REPORT_LO		:=		report.lo

REPORT_C		:=		report.c

REPORT_O		:=		$(REPORT_C:.c=.o)

REPORT_INCLUDE		:=		$(directory)/include/report.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(REPORT_LO)

$(REPORT_LO):		$(REPORT_O)
	$(call env_remove,$(REPORT_LO),)

	$(call env_archive,$(REPORT_LO),$(REPORT_O),)

clear:
	$(call env_remove,$(REPORT_O),)

	$(call env_remove,$(REPORT_LO),)

	$(call env_purge,)

prototypes:

	$(call env_prototypes,$(REPORT_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(REPORT_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
