##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/tools/ctcompare/Makefile
##
## created       julien quintard   [tue feb 21 23:02:19 2006]
## updated       julien quintard   [thu feb 23 06:48:17 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

CTBUILD_C		:=		buildctf.c			\
					clexer.c			

CTBUILD_O		:=		$(CTBUILD_C:.c=.o)

CTCOMPARE_C		:=		ctcompare.c			

CTCOMPARE_O		:=		$(CTCOMPARE_C:.c=.o)

override _CFLAGS_	:=		

#
# ---------- rules ------------------------------------------------------------
#

all:			$(_CTBUILD_TOOL_)

$(_CTBUILD_TOOL_):	$(CTBUILD_O)
	cc $(CTBUILD_O) -o $(_CTBUILD_TOOL_) # XXX strace

$(_CTCOMPARE_TOOL_):	$(CTCOMPARE_O)
	cc $(CTCOMPARE_O) -o $(_CTCOMPARE_TOOL_) # XXX strace

clear:
	$(call remove,$(_CTBUILD_TOOL_) $(_CTCOMPARE_TOOL_),)

	$(call remove,$(CTBUILD_O) $(CTCOMPARE_O),)

	$(call purge,)

proto:

dep:
