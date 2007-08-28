#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/kaneton/core/message/Makefile
#
# created       julien quintard   [thu jun 28 20:34:12 2007]
# updated       matthieu bucchianeri   [mon aug 27 22:25:35 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/message

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

MESSAGE_C		:=		message.c

MESSAGE_O		:=		$(MESSAGE_C:.c=.o)

MESSAGE_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/message.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_MESSAGE_LO_)

$(_MESSAGE_LO_):	$(MESSAGE_O) $(_LIBC_A_)
	$(call env_remove,$(_MESSAGE_LO_),)

	$(call env_archive,$(_MESSAGE_LO_),$(MESSAGE_O),)

clear:
	$(call env_remove,$(MESSAGE_O),)

	$(call env_remove,$(_MESSAGE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(MESSAGE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(MESSAGE_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
