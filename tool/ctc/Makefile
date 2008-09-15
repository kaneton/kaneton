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

CTC_BUILD_C		:=		buildctf.c			\
					clexer.c			
CTC_BUILD_O		:=		$(CTC_BUILD_C:.c=.o)

CTC_GATHER_C		:=		enhashctf.c			\
					hctffiles.c			\
					tokens.c			
CTC_GATHER_O		:=		$(CTC_GATHER_C:.c=.o)

CTC_COMPARE_C		:=		ctcompare.c			\
					hctffiles.c			\
					tokens.c			\
					avltree.c			
CTC_COMPARE_O		:=		$(CTC_COMPARE_C:.c=.o)

CTC_SHOW_C		:=		showkeys.c			\
					tokens.c			\
					hctffiles.c			
CTC_SHOW_O		:=		$(CTC_SHOW_C:.c=.o)

CTC_DETOK_C		:=		detok.c				\
					tokens.c			
CTC_DETOK_O		:=		$(CTC_DETOK_C:.c=.o)

override _CC_FLAGS_	:=		-DHCTF

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_CTC_BUILD_TOOL_) $(_CTC_GATHER_TOOL_)	\
			$(_CTC_COMPARE_TOOL_) $(_CTC_SHOW_TOOL_)	\
			$(_CTC_DETOK_TOOL_)

$(_CTC_BUILD_TOOL_):	$(CTC_BUILD_O)
	$(call env_executable,$(_CTC_BUILD_TOOL_),			\
			      $(CTC_BUILD_O),,				\
			      $(ENV_OPTION_STANDARD))

$(_CTC_GATHER_TOOL_):	$(CTC_GATHER_O)
	$(call env_executable,$(_CTC_GATHER_TOOL_),			\
			      $(CTC_GATHER_O),,				\
			      $(ENV_OPTION_STANDARD))

$(_CTC_COMPARE_TOOL_):	$(CTC_COMPARE_O)
	$(call env_executable,$(_CTC_COMPARE_TOOL_),			\
			      $(CTC_COMPARE_O),,			\
			      $(ENV_OPTION_STANDARD))

$(_CTC_SHOW_TOOL_):	$(CTC_SHOW_O)
	$(call env_executable,$(_CTC_SHOW_TOOL_),			\
			      $(CTC_SHOW_O),,				\
			      $(ENV_OPTION_STANDARD))

$(_CTC_DETOK_TOOL_):	$(CTC_DETOK_O)
	$(call env_executable,$(_CTC_DETOK_TOOL_),			\
			      $(CTC_DETOK_O),,				\
			      $(ENV_OPTION_STANDARD))

clear:
	$(call env_remove,$(_CTC_BUILD_TOOL_) $(CTC_BUILD_O),)
	$(call env_remove,$(_CTC_GATHER_TOOL_) $(CTC_GATHER_O),)
	$(call env_remove,$(_CTC_COMPARE_TOOL_) $(CTC_COMPARE_O),)
	$(call env_remove,$(_CTC_SHOW_TOOL_) $(CTC_SHOW_O),)
	$(call env_remove,$(_CTC_DETOK_TOOL_) $(CTC_DETOK_O),)

	$(call env_purge,)

prototypes:

headers:
