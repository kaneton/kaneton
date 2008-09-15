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

TETON			:=		teton

TETON_C			:=		teton.c

TETON_O			:=		$(TETON_C:.c=.o)

#XXX := /home/mycure/kayou/library/kayou/d/compilers/dmd/test/_crt.o /home/mycure/kayou/library/kayou/d/compilers/dmd/test/_kayou.o /home/mycure/kayou/library/kayou/d/compilers/dmd/test/test.o /home/mycure/kayou/library/kayou/d/compilers/dmd/libkayou.a
#XXX := /home/mycure/kayou/library/kayou/d/compilers/dmd/test/_crt.o /home/mycure/kayou/library/kayou/d/compilers/dmd/test/_kayou.o

XXX := $(_CRT_O_) /home/mycure/kayou/library/kayou/d/compilers/dmd/libkayou.a /home/mycure/kayou/library/kayou/d/compilers/dmd/test/test.o $(_ULIBC_A_)

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(TETON)

$(TETON):			$(TETON_O)
	$(call env_executable,$(TETON),					\
			      $(TETON_O)				\
			      $(_LIBKANETON_A_) \
			      $(XXX), \
			      $(_DRIVER_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(TETON_O),)
	$(call env_remove,$(TETON),)

	$(call env_purge,)

prototypes:

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TETON_C),)

dependencies:
	$(call env_launch,$(_ULIBC_DIR_)/Makefile,,)
	$(call env_launch,$(_LIBKANETON_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
