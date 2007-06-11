##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/environment/Makefile
##
## created       julien quintard   [fri feb 11 02:59:20 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:42:31 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main initialize clean clear prototypes headers

#
# ---------- rules ------------------------------------------------------------
#

main:			initialize

initialize:
	$(call env_launch,$(_ENVIRONMENT_DIR_)/initialize.py,,)

clean:
	$(call env_launch,$(_ENVIRONMENT_DIR_)/clean.py,,)

clear:
	$(call env_purge,)

prototypes:

headers:
