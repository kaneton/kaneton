##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/libs/crt
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Mon Nov  7 15:40:19 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

CRT_C			:=		crt.c				

CRT_O			:=		$(CRT_C:.c=.o)

CRT_INCLUDE		:=		

#
# ---------- rules ------------------------------------------------------------
#

$(_CRT_A_):		$(CRT_O)
	$(call static-library,$(_CRT_A_),$(CRT_O),)

clear:
	$(call remove,$(CRT_O),)

	$(call remove,$(_CRT_A_),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CRT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
