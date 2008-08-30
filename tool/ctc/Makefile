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

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

CTC_BUILD_C		:=		build.c				\
					lexer.c			
CTC_BUILD_O		:=		$(CTC_BUILD_C:.c=.o)

CTC_COMPARE_C		:=		compare.c			
CTC_COMPARE_O		:=		$(CTC_COMPARE_C:.c=.o)

override _CC_FLAGS_	:=		

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_CTC_BUILD_TOOL_) $(_CTC_COMPARE_TOOL_)

$(_CTC_BUILD_TOOL_):	$(CTC_BUILD_O)
	$(call env_executable,$(_CTC_BUILD_TOOL_),			\
			      $(CTC_BUILD_O),,				\
			      $(ENV_OPTION_STANDARD))

$(_CTC_COMPARE_TOOL_):	$(CTC_COMPARE_O)
	$(call env_executable,$(_CTC_COMPARE_TOOL_),			\
			      $(CTC_COMPARE_O),,			\
			      $(ENV_OPTION_STANDARD))

clear:
	$(call env_remove,$(_CTC_BUILD_TOOL_) $(_CTC_COMPARE_TOOL_),)

	$(call env_remove,$(CTC_BUILD_O) $(CTC_COMPARE_O),)

	$(call env_purge,)

prototypes:

headers:
