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

include			../../env/.env.mk

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
	$(call executable,$(MOD1),$(MOD1_O) $(COMMON_O) $(_KLIBC_A_),	\
	         "--link-script $(_MACHDEP_LINK_DIR_)/driver.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

$(MOD2):		libs $(COMMON_O) $(MOD2_O)
	$(call executable,$(MOD2),$(MOD2_O) $(COMMON_O) $(_KLIBC_A_),	\
	         "--link-script $(_MACHDEP_LINK_DIR_)/driver.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")
libs:
	$(call launch,Makefile,$(_KLIBC_DIR_),,)

clear:
	$(call remove,$(MOD1_O),)
	$(call remove,$(MOD1),)
	$(call remove,$(MOD2_O),)
	$(call remove,$(MOD2),)
	$(call remove,$(COMMON_O),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MOD1_C),$(_MAKEFILE_MK_),)

	$(call dependencies,$(MOD2_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
