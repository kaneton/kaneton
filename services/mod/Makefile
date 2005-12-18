##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/services/mod/Makefile
##
## created       julien quintard   [fri feb 11 03:00:09 2005]
## updated       matthieu bucchianeri   [sun dec 18 12:04:51 2005]
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

MOD			:=		mod

MOD_C			:=		mod.c				\
					modfs.c

MOD_O			:=		$(MOD_C:.c=.o)

MOD_INCLUDE		:=

#
# ---------- rules ------------------------------------------------------------
#

$(MOD):			libs $(MOD_O)
	$(call executable,$(MOD),$(MOD_O) $(_KLIBC_A_),			\
	  "--lds $(_MACHDEP_LDS_DIR_)/service.lds")

libs:
	$(call make,$(_KLIBC_DIR_),)

clear:
	$(call remove,$(MOD_O),)
	$(call remove,$(MOD),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MOD_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
