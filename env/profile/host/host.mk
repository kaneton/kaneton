#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/env/profile/environment/environment.mk
#
# created       julien quintard   [tue may  8 13:01:15 2007]
# updated       julien quintard   [thu may 10 19:26:05 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this file contains general stuff about the environment.
#

#
# ---------- wrapper ----------------------------------------------------------
#

define env
  $(call $(1),$(2),$(3),$(4),$(5),$(6),$(7),$(8),$(9))
endef
