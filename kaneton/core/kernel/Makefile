##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/kernel/Makefile
##
## created       julien quintard   [sat dec 10 13:50:17 2005]
## updated       julien quintard   [sat jun 24 13:33:23 2006]
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

KERNEL_LO		:=		kernel.lo

KERNEL_C		:=		kernel.c			

KERNEL_O		:=		$(KERNEL_C:.c=.o)

KERNEL_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kernel.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(KERNEL_LO):		$(KERNEL_O)
	$(call remove,$(KERNEL_LO),)

	$(call archive,$(KERNEL_LO),$(KERNEL_O),)

clear:
	$(call remove,$(KERNEL_O),)

	$(call remove,$(KERNEL_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(KERNEL_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(KERNEL_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
