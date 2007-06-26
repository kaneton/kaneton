##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/message/Makefile
##
## created       matthieu bucchianeri   [sat jun 17 19:36:16 2006]
## updated       matthieu bucchianeri   [sat may  5 18:43:15 2007]
##

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

MESSAGE_C		:=		message.c		\
					node.c

MESSAGE_O		:=		$(MESSAGE_C:.c=.o)

MESSAGE_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/message.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

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

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
