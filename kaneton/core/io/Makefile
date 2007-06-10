##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/io/Makefile
##
## created       matthieu bucchianeri   [sat jun 17 19:36:16 2006]
## updated       matthieu bucchianeri   [sat may  5 18:42:50 2007]
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

IO_LO			:=		io.lo

IO_C			:=		io.c

IO_O			:=		$(IO_C:.c=.o)

IO_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/io.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(IO_LO):		$(IO_O)
	$(call env_remove,$(IO_LO),)

	$(call env_archive,$(IO_LO),$(IO_O),)

clear:
	$(call env_remove,$(IO_O),)

	$(call env_remove,$(IO_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(IO_INCLUDE),)

dep:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(IO_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
