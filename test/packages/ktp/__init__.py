#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/__init__.py
#
# created       julien quintard   [mon oct 25 19:43:41 2010]
# updated       julien quintard   [wed feb 16 13:31:24 2011]
#

#
# ---------- information ------------------------------------------------------
#
# the kaneton test package contains everything necessary for the scripts
# to perform cryptographic tasks, issuing remote requests and so on.
#
# note that this package is even more important for the server script which
# evolves outside the kaneton environment, hence does not have access to
# the kaneton environment functionalities. this is why this package provides
# features for reading and writing files for instance.
#

#
# ---------- definitions ------------------------------------------------------
#

StatusOk = True
StatusError = False
StatusUnknown = -666

#
# ---------- modules ----------------------------------------------------------
#

import bulletin
import capability
import certificate
import code
import configuration
import database
import environment
import hook
import key
import log
import manifest
import miscellaneous
import process
import queue
import report
import snapshot
import stage
import statement
import suite
import trace
import xmlrpc
