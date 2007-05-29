#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/configure/configure.frm
#
# created       julien quintard   [fri may 25 09:51:05 2007]
# updated       julien quintard   [tue may 29 20:52:05 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this is the main frame which contains all the subframe entries.
#

#
# ---------- general ----------------------------------------------------------
#

- title: Configure
  description: This is the main menu of the kaneton configure utility.

#
# ---------- frames -----------------------------------------------------------
#

- frame: Boot
  path: boot/boot.frm

- frame: Core
  path: core/core.frm

#
# ---------- variables --------------------------------------------------------
#

- variable: _DISPLAY_

- variable: _SEGMENT_LOOKUP_ALGORITHM_
