##
## licence       kaneton licence
##
## project       knaeton
##
## file          /home/buckman/kaneton/core/kaneton/conf/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:39:41 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

CONF_LO			:=		conf.lo

CONF_C			:=		conf.c

CONF_O			:=		$(CONF_C:.c=.o)

CONF_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/conf.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(CONF_LO):		$(CONF_O)
	$(call remove,$(CONF_LO),)

	$(call archive,$(CONF_LO),$(CONF_O),)

clear:
	$(call remove,$(CONF_O),)

	$(call remove,$(CONF_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(CONF_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CONF_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
