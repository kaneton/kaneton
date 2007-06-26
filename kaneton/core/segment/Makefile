#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/segment/Makefile
#
# created       julien quintard   [sun jun 10 17:02:46 2007]
# updated       julien quintard   [tue jun 26 15:51:45 2007]
#

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

SEGMENT_C		:=		segment.c			\
					segment-fit.c			

SEGMENT_O		:=		$(SEGMENT_C:.c=.o)

SEGMENT_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/segment.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			dependencies $(_SEGMENT_LO_)

$(_SEGMENT_LO_):	$(SEGMENT_O) $(_LIBC_A_)
	$(call env_remove,$(_SEGMENT_LO_),)

	$(call env_archive,$(_SEGMENT_LO_),$(SEGMENT_O),)

clear:
	$(call env_remove,$(SEGMENT_O),)

	$(call env_remove,$(_SEGMENT_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(SEGMENT_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(SEGMENT_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
