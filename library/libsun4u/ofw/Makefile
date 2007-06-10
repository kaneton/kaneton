##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/libsun4u/ofw/Makefile
##
## created       matthieu bucchianeri   [thu aug 10 16:01:50 2006]
## updated       matthieu bucchianeri   [thu aug 10 16:02:35 2006]
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

OFW_LO			:=		ofw.lo

OFW_C			:=		ofw.c

OFW_S			:=		openfw.S

OFW_O			:=		$(OFW_C:.c=.o)			\
					$(OFW_S:.S=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(OFW_LO):		$(OFW_O)
	$(call env_remove,$(OFW_LO),)

	$(call env_archive,$(OFW_LO),$(OFW_O),)

clear:
	$(call env_remove,$(OFW_O),)

	$(call env_remove,$(OFW_LO),)

	$(call env_purge,)

dep:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(OFW_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
