#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/export/modules/initenv.py
#
# created       francois goudal   [sat oct 25 20:57:38 2008]
# updated       julien quintard   [mon apr 20 15:03:29 2009]
#

#
# ---------- imports ----------------------------------------------------------
#

import env

#
# ---------- functions --------------------------------------------------------
#

#
# module_init()
#
# called by export.py on startup, to discover the available actions
# returns an array of strings, the index 0 is the action name, and the
# others are the parameters required in the YAML file.
#
def module_init():
    return ['initenv']

#
# module_action(export_dir, arg)
#
# called by export.py when required by the YAML file.
# this function does the job of the action.
# arg contains the parameters required as specified in the module_init return
# value.
#
def module_action(export_dir, arg):
    env.display(env.HEADER_OK, 'action initenv', env.OPTION_NONE)
    env.launch(export_dir + '/environment/initialize.py', "", env.OPTION_QUIET)
    env.launch(export_dir + '/Makefile', "clean", env.OPTION_QUIET)
    return 0
