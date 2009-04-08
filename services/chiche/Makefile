##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/drivers/mod/Makefile
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

CHICHE				:=		chiche

CHICHE_C			:=		chiche.c
CHICHE_O			:=		$(CHICHE_C:.c=.o)

CRT_S				:=		_crt.S
CRT_O				:=		$(CRT_S:.S=.o)

KAYOU_C				:=		_kayou.c
KAYOU_O				:=		$(KAYOU_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(CHICHE)

$(CHICHE):			$(CRT_O) $(KAYOU_O) $(CHICHE_O) $(_LIBKANETON_A_)
	$(call env_executable,$(CHICHE),				\
			      $(CRT_O)	 				\
			      $(KAYOU_O) 				\
			      $(CHICHE_O)				\
			      $(_LIBKANETON_A_),			\
			      $(_DRIVER_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(CRT_O),)
	$(call env_remove,$(KAYOU_O),)
	$(call env_remove,$(CHICHE_O),)
	$(call env_remove,$(CHICHE),)

	$(call env_purge,)

prototypes:

headers:

dependencies:
	$(call env_launch,$(_LIBKANETON_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
