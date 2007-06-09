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

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

CPU_LO			:=		cpu.lo

CPU_C			:=		cpu.c

CPU_O			:=		$(CPU_C:.c=.o)

CPU_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/cpu.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(CPU_LO):		$(CPU_O)
	$(call env_remove,$(CPU_LO),)

	$(call env_archive,$(CPU_LO),$(CPU_O),)

clear:
	$(call env_remove,$(CPU_O),)

	$(call env_remove,$(CPU_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(CPU_INCLUDE),)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(CPU_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
