##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/drivers/syscall/Makefile
##
## created       julien quintard   [fri feb 11 03:00:09 2005]
## updated       matthieu bucchianeri   [sat may  5 18:22:24 2007]
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

SYSCALL			:=		syscall

SYSCALL_C		:=		syscall.c

SYSCALL_O		:=		$(SYSCALL_C:.c=.o)

SYSCALL_INCLUDE		:=

#
# ---------- rules ------------------------------------------------------------
#

$(SYSCALL):		libs $(SYSCALL_O)
	$(call env_executable,$(SYSCALL),$(SYSCALL_O) $(_KLIBC_A_),		\
	         "--link-script $(_MACHDEP_LINK_DIR_)/driver.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

libs:
	$(call env_launch,Makefile,$(_KLIBC_DIR_),,)

clear:
	$(call env_remove,$(SYSCALL_O),)
	$(call env_remove,$(SYSCALL),)

	$(call env_purge,)

proto:

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(SYSCALL_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
