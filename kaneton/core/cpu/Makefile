##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/cpu/Makefile
##
## created       matthieu bucchianeri   [sat jun 17 19:36:16 2006]
## updated       matthieu bucchianeri   [sat jul 29 17:59:21 2006]
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

CPU_LO			:=		cpu.lo

CPU_C			:=		cpu.c		\
					$(_CPU_FILE_)

CPU_O			:=		$(CPU_C:.c=.o)

CPU_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/cpu.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(CPU_LO):		$(CPU_O)
	$(call remove,$(CPU_LO),)

	$(call archive,$(CPU_LO),$(CPU_O),)

clear:
	$(call remove,$(CPU_O),)

	$(call remove,$(CPU_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(CPU_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CPU_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
