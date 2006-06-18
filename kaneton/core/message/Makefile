##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/message/Makefile
##
## created       matthieu bucchianeri   [sat jun 17 19:36:16 2006]
## updated       matthieu bucchianeri   [sat jun 17 19:36:42 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

MESSAGE_LO		:=		message.lo

MESSAGE_C		:=		message.c		\
					$(_MESSAGE_FILE_)

MESSAGE_O		:=		$(MESSAGE_C:.c=.o)

MESSAGE_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/message.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(MESSAGE_LO):		$(MESSAGE_O)
	$(call remove,$(MESSAGE_LO),)

	$(call archive,$(MESSAGE_LO),$(MESSAGE_O),)

clear:
	$(call remove,$(MESSAGE_O),)

	$(call remove,$(MESSAGE_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(MESSAGE_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MESSAGE_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
