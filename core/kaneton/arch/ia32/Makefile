##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/arch/ia32
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Tue Aug 30 13:46:41 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		ia32 kaneton

MACHDEP_A		:=		machdep.a

KANETON_A		:=		kaneton/kaneton.a
IA32_A			:=		ia32/ia32.a

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(MACHDEP_A):		libs
	$(call archives,$(MACHDEP_A),$(KANETON_A) $(IA32_A),)

libs:
	$(call make,$(SUBDIRS),)

clear:
	$(call make,$(SUBDIRS),clear)

	$(call remove,$(MACHDEP_A),)

	$(call purge,)

proto:
	$(call make,$(SUBDIRS),proto)

dep:
	$(call make,$(SUBDIRS),dep)
