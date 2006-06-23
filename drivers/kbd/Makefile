##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/drivers/kbd/Makefile
##
## created       julien quintard   [fri feb 11 03:00:09 2005]
## updated       julien quintard   [fri jun 23 16:24:46 2006]
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

KBD			:=		kbd

KBD_C			:=		kbd.c

KBD_O			:=		$(KBD_C:.c=.o)

KBD_INCLUDE		:=

#
# ---------- rules ------------------------------------------------------------
#

$(KBD):			libs $(KBD_O)
	$(call executable,$(KBD),$(KBD_O) $(_KLIBC_A_),			\
                 "--link-script $(_MACHDEP_LINK_DIR_)/driver.lks"	\
                 "--no-standard-include"				\
                 "--no-standard-library")

libs:
	$(call make,$(_KLIBC_DIR_),)

clear:
	$(call remove,$(KBD_O),)
	$(call remove,$(KBD),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(KBD_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
