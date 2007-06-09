##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/libia32/Makefile
##
## created       matthieu bucchianeri   [tue dec 20 13:40:52 2005]
## updated       matthieu bucchianeri   [tue jul 25 23:08:33 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep libs

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		pmode				\
					paging				\
					interrupt			\
					time				\
					task				\
					apic				\
					misc

PMODE_LO		:=		pmode/pmode.lo
PAGING_LO		:=		paging/paging.lo
INTERRUPT_LO		:=		interrupt/interrupt.lo
TIME_LO			:=		time/time.lo
TASK_LO			:=		task/task.lo
APIC_LO			:=		apic/apic.lo
MISC_LO			:=		misc/misc.lo

LIBIA32_INCLUDE		:=		$(_LIBIA32_INCLUDE_DIR_)/libia32.h

#
# ---------- rules ------------------------------------------------------------
#

libs:
	$(call env_launch,Makefile,$(SUBDIRS),,)

	$(call env_launch,Makefile,.,$(_LIBIA32_A_),)

$(_LIBIA32_A_):		$(PMODE_LO) $(PAGING_LO) $(INTERRUPT_LO)	\
			$(TIME_LO) $(TASK_LO) $(APIC_LO) $(MISC_LO)
	$(call env_remove,$(_LIBIA32_A_),)

	$(call env_static-library,$(_LIBIA32_A_),$(PMODE_LO) $(PAGING_LO)	\
				$(INTERRUPT_LO) $(TIME_LO) $(TASK_LO)	\
				$(APIC_LO) $(MISC_LO),)

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(LIBIA32_O),)

	$(call env_remove,$(_LIBIA32_A_),)

	$(call env_purge,)

proto:
	$(call env_launch,Makefile,$(SUBDIRS),proto,)

	$(call env_prototypes,$(LIBIA32_INCLUDE),)

dep:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
