##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/cpu/Makefile
##
## created       matthieu bucchianeri   [sat jun 17 19:36:16 2006]
## updated       matthieu bucchianeri   [sat may  5 18:42:39 2007]
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

CPU_C			:=		cpu.c

CPU_O			:=		$(CPU_C:.c=.o)

CPU_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/cpu.h

# XXX: ugly
CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_CPU_LO_)

$(_CPU_LO_):		$(CPU_O)
	$(call env_remove,$(_CPU_LO_),)

	$(call env_archive,$(_CPU_LO_),$(CPU_O),)

clear:
	$(call env_remove,$(CPU_O),)

	$(call env_remove,$(_CPU_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(CPU_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(CPU_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
