##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/tools/ctcompare/Makefile
##
## created       julien quintard   [tue feb 21 23:02:19 2006]
## updated       julien quintard   [sat feb 25 16:42:46 2006]
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

all:			$(_CTBUILD_TOOL_) $(_CTCOMPARE_TOOL_)

$(_CTBUILD_TOOL_):	$(CTBUILD_O)
	$(call executable,$(_CTBUILD_TOOL_),$(CTBUILD_O),)

$(_CTCOMPARE_TOOL_):	$(CTCOMPARE_O)
	$(call executable,$(_CTCOMPARE_TOOL_),$(CTCOMPARE_O),)

clear:
	$(call remove,$(_CTBUILD_TOOL_) $(_CTCOMPARE_TOOL_),)

	$(call remove,$(CTBUILD_O) $(CTCOMPARE_O),)

	$(call purge,)

proto:

dep:
