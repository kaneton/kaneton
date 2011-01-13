#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...e/architecture/ia32/educational/Makefile
#
# created       julien quintard   [sun dec 10 18:38:17 2006]
# updated       julien quintard   [tue jan 11 17:11:26 2011]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/machine/architecture

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

ARCHITECTURE_C		:=	context.c				\
				gdt.c					\
				handler.c				\
				idt.c					\
				io.c					\
				pmode.c					\
				tlb.c					\
				tss.c					\
				paging.c				\
\
				mapping.c				\
				pd.c					\
				pt.c					\
				segmentation.c				\
				virtual.c				

ARCHITECTURE_O		:=	$(ARCHITECTURE_C:.c=.o)

ARCHITECTURE_INCLUDE	:=	$(_ARCHITECTURE_INCLUDE_DIR_)/context.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/gdt.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/handler.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/idt.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/io.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/paging.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/tlb.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/tss.h	\
\
				$(_ARCHITECTURE_INCLUDE_DIR_)/mapping.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/misc.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/pmode.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/segmentation.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/virtual.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_ARCHITECTURE_LO_)

$(_ARCHITECTURE_LO_):		$(ARCHITECTURE_O)
	$(call env_remove,$(_ARCHITECTURE_LO_),)

	$(call env_archive,$(_ARCHITECTURE_LO_),$(ARCHITECTURE_O),)

dependencies:

clear:
	$(call env_remove,$(ARCHITECTURE_O),)

	$(call env_remove,$(_ARCHITECTURE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(ARCHITECTURE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(ARCHITECTURE_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
