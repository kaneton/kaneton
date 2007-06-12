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

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

IO_C			:=		io.c

IO_O			:=		$(IO_C:.c=.o)

IO_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/io.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_IO_LO_)

$(_IO_LO_):		$(IO_O)
	$(call env_remove,$(_IO_LO_),)

	$(call env_archive,$(_IO_LO_),$(IO_O),)

clear:
	$(call env_remove,$(IO_O),)

	$(call env_remove,$(_IO_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(IO_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(IO_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
