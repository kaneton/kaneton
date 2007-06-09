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

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

MESSAGE_LO		:=		message.lo

MESSAGE_C		:=		message.c		\
					node.c

MESSAGE_O		:=		$(MESSAGE_C:.c=.o)

MESSAGE_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/message.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(MESSAGE_LO):		$(MESSAGE_O)
	$(call env_remove,$(MESSAGE_LO),)

	$(call env_archive,$(MESSAGE_LO),$(MESSAGE_O),)

clear:
	$(call env_remove,$(MESSAGE_O),)

	$(call env_remove,$(MESSAGE_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(MESSAGE_INCLUDE),)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(MESSAGE_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
