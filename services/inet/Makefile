#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/services/inet/Makefile
#
# created       matthieu bucchianeri   [fri sep  7 23:29:54 2007]
# updated       matthieu bucchianeri   [sun sep  9 23:22:15 2007]
#

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

INET			:=		inet

INET_C			:=		inet.c				\
					hexo/timer.c			\
					libnetwork/packet.c		\
					libnetwork/if.c			\
					libnetwork/protocols.c		\
					libnetwork/filter.c		\
					libnetwork/route.c		\
					libnetwork/protocols/arp.c	\
					libnetwork/protocols/ipv4.c	\
					libnetwork/protocols/icmpv4.c	\
					libnetwork/protocols/udp.c	\
					libnetwork/libudp/libudp.c

INET_O			:=		$(INET_C:.c=.o)

INET_INCLUDE		:=		$(_SERVICES_DIR_)/inet/inet.h

_CC_FLAGS_		+=	-I$(_SERVICES_DIR_)/inet/libnetwork/include \
				-I$(_SERVICES_DIR_)/inet/gpct/include	\
				-I$(_SERVICES_DIR_)/inet/hexo/include	\
				-include config.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(INET)

$(INET):			libs $(INET_O)
	$(call env_executable,$(INET),					\
			      $(INET_O)					\
			      $(_CRT_O_) $(_ULIBC_A_) $(_LIBPTHREAD_A_)	\
			      $(_LIBKANETON_A_),			\
			      $(_SERVICE_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

libs:
	$(call env_launch,${_ULIBC_DIR_}/Makefile,,)
	$(call env_launch,${_LIBKANETON_DIR_}/Makefile,,)
	$(call env_launch,${_LIBPTHREAD_DIR_}/Makefile,,)

clear:
	$(call env_remove,$(INET_O),)
	$(call env_remove,$(INET),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(INET_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(INET_C),)

dependencies:
	$(call env_launch,$(_ULIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
