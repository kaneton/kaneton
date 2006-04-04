##
## licence       Kaneton licence
##
## project       kaneton
##
## file          /home/rhino/kaneton/kaneton/core/thread/Makefile
##
## created       renaud voltz   [tue apr  4 03:02:16 2006]
## updated       renaud voltz   [tue apr  4 03:02:16 2006]
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

THREAD_LO		:=		thread.lo

THREAD_C		:=		thread.c

THREAD_O		:=		$(THREAD_C:.c=.o)

THREAD_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/thread.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(THREAD_LO):		$(THREAD_O)
	$(call remove,$(THREAD_LO),)

	$(call archive,$(THREAD_LO),$(THREAD_O),)

clear:
	$(call remove,$(THREAD_O),)

	$(call remove,$(THREAD_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(THREAD_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(THREAD_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
