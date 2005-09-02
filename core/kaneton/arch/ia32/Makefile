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
## last update   Wed Aug 31 16:41:15 2005   mycure
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

MACHDEP_LO		:=		machdep.lo

KANETON_LO		:=		kaneton/kaneton.lo
IA32_LO			:=		ia32/ia32.lo

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(MACHDEP_LO):		libs
	$(call remove,$(MACHDEP_LO),)

	$(call archive,$(MACHDEP_LO),$(KANETON_LO) $(IA32_LO),)

libs:
	$(call make,$(SUBDIRS),)

clear:
	$(call make,$(SUBDIRS),clear)

	$(call remove,$(MACHDEP_LO),)

	$(call purge,)

proto:
	$(call make,$(SUBDIRS),proto)

dep:
	$(call make,$(SUBDIRS),dep)
