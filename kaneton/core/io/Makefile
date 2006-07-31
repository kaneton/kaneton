##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/io/Makefile
##
## created       matthieu bucchianeri   [sat jun 17 19:36:16 2006]
## updated       matthieu bucchianeri   [sat jul 29 18:02:23 2006]
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

IO_LO			:=		io.lo

IO_C			:=		io.c		\
					$(_IO_FILE_)

IO_O			:=		$(IO_C:.c=.o)

IO_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/io.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(IO_LO):		$(IO_O)
	$(call remove,$(IO_LO),)

	$(call archive,$(IO_LO),$(IO_O),)

clear:
	$(call remove,$(IO_O),)

	$(call remove,$(IO_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(IO_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(IO_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
