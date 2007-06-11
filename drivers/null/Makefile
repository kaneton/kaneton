##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/drivers/null/Makefile
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

NULL			:=		null

NULL_C			:=		null.c

NULL_O			:=		$(NULL_C:.c=.o)

NULL_INCLUDE		:=

#
# ---------- rules ------------------------------------------------------------
#

$(NULL):		libs $(NULL_O)
	$(call env_executable,$(NULL),$(NULL_O) $(_KLIBC_A_),		\
	         "--link-script $(_MACHDEP_LINK_DIR_)/driver.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

libs:
	$(call env_launch,Makefile,$(_KLIBC_DIR_),,)

clear:
	$(call env_remove,$(NULL_O),)
	$(call env_remove,$(NULL),)

	$(call env_purge,)

proto:

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(NULL_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
