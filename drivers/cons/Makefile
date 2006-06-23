##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/drivers/cons/Makefile
##
## created       julien quintard   [fri feb 11 03:00:09 2005]
## updated       julien quintard   [fri jun 23 16:24:58 2006]
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

CONS			:=		cons

CONS_C			:=		cons.c

CONS_O			:=		$(CONS_C:.c=.o)

CONS_INCLUDE		:=

#
# ---------- rules ------------------------------------------------------------
#

$(CONS):		libs $(CONS_O)
	$(call executable,$(CONS),$(CONS_O) $(_KLIBC_A_),		\
	         "--link-script $(_MACHDEP_LINK_DIR_)/driver.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

libs:
	$(call make,$(_KLIBC_DIR_),)

clear:
	$(call remove,$(CONS_O),)
	$(call remove,$(CONS),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CONS_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
