#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane.../loader/ibm-pc.ia32/educational/Makefile
#
# created       julien quintard   [tue jun 12 20:34:41 2007]
# updated       julien quintard   [tue nov 23 18:16:03 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		loader

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

LOADER_C		:=		loader.c			\
					cons.c				\
					init.c				\
					paging.c			\
					pmode.c				\
					libia32.c			\
					libc.c				\
					early.c

LOADER_S		:=		multiboot.S			\
					entry.S

LOADER_O		:=		$(LOADER_C:.c=.o)		\
					$(LOADER_S:.S=.o)

LOADER_INCLUDE		:=		$(_LOADER_DIR_)/loader.h 	\
					$(_LOADER_DIR_)/cons.h		\
					$(_LOADER_DIR_)/libia32.h	\
					$(_LOADER_DIR_)/libc.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_LOADER_)

$(_LOADER_):		$(LOADER_O)
	$(call env_executable,$(_LOADER_),				\
			      $(LOADER_O),				\
			      $(_LOADER_DIR_)/layout/loader.lyt,\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(LOADER_O),)

	$(call env_remove,$(_LOADER_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(LOADER_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(LOADER_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
