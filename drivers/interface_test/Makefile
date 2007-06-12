##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/drivers/interface_test/Makefile
##
## created       julien quintard   [fri feb 11 03:00:09 2005]
## updated       matthieu bucchianeri   [tue may 22 00:56:41 2007]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

INTERFACE		:=		interface

INTERFACE_C		:=		interface.c		\
					interface_user.c

INTERFACE_O		:=		$(INTERFACE_C:.c=.o)

INTERFACE_INCLUDE	:=


#
# ---------- rules ------------------------------------------------------------
#

$(INTERFACE):		libs $(INTERFACE_O)
	$(call env_executable,$(INTERFACE),$(INTERFACE_O) $(_KLIBC_A_),	\
	         "--link-script $(_MACHDEP_LINK_DIR_)/driver.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

libs:
	$(call env_launch,Makefile,$(_KLIBC_DIR_),,)

clear:
	$(call env_remove,$(INTERFACE_O),)
	$(call env_remove,$(INTERFACE),)

	$(call env_purge,)

proto:
	$(call env_prototypes,interface_user.h,)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(INTERFACE_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
