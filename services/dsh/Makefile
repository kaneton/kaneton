##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/services/dsh/Makefile
##
## created       julien quintard   [fri feb 11 03:00:09 2005]
## updated       julien quintard   [sat feb 25 14:46:21 2006]
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

DSH			:=		dsh

DSH_C			:=		dsh.c

DSH_O			:=		$(DSH_C:.c=.o)

DSH_INCLUDE		=

#
# ---------- rules ------------------------------------------------------------
#

$(DSH):			libs $(DSH_O)
	$(call executable,$(DSH),$(DSH_O) $(_KLIBC_A_),			\
                 "--ld-script $(_MACHDEP_LDS_DIR_)/service.lds"		\
                 "--no-standard-include"				\
                 "--no-standard-library")

libs:
	$(call makefile,$(_KLIBC_DIR_),)

clear:
	$(call remove,$(DSH_O),)
	$(call remove,$(DSH),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(DSH_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
