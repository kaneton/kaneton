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

MOD1			:=		mod1

MOD2			:=		mod2

MOD1_C			:=		mod1.c

MOD2_C			:=		mod2.c

COMMON_C		:=		common.c

MOD1_O			:=		$(MOD1_C:.c=.o)

MOD2_O			:=		$(MOD2_C:.c=.o)

COMMON_O		:=		$(COMMON_C:.c=.o)

MOD1_INCLUDE		:=

MOD2_INCLUDE		:=

COMMON_INCLUDE		:=

#
# ---------- rules ------------------------------------------------------------
#

message_test:	$(MOD1) $(MOD2)

$(MOD1):		libs $(COMMON_O) $(MOD1_O)
	$(call env_executable,$(MOD1),$(MOD1_O) $(COMMON_O) $(_KLIBC_A_),	\
	         "--link-script $(_MACHDEP_LINK_DIR_)/driver.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

$(MOD2):		libs $(COMMON_O) $(MOD2_O)
	$(call env_executable,$(MOD2),$(MOD2_O) $(COMMON_O) $(_KLIBC_A_),	\
	         "--link-script $(_MACHDEP_LINK_DIR_)/driver.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")
libs:
	$(call env_launch,Makefile,$(_KLIBC_DIR_),,)

clear:
	$(call env_remove,$(MOD1_O),)
	$(call env_remove,$(MOD1),)
	$(call env_remove,$(MOD2_O),)
	$(call env_remove,$(MOD2),)
	$(call env_remove,$(COMMON_O),)

	$(call env_purge,)

proto:

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(MOD1_C),)

	$(call env_headers,$(MOD2_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
