#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/export/modules/freplace.py
#
# created       francois goudal   [sat oct 25 20:57:38 2008]
# updated       julien quintard   [sat feb  5 11:32:56 2011]
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
    return ['freplace','src','dst']

#
# module_action(export_dir, arg)
#
# called by export.py when required by the YAML file.
# this function does the job of the action.
# arg contains the parameters required as specified in the module_init return
# value.
#
def module_action(export_dir, arg):
    env.display(env.HEADER_OK, 'action freplace ' + arg['src'] + ' to ' + arg['dst'], env.OPTION_NONE)
    env.copy(export_dir + '/' + arg['src'], export_dir + '/' + arg['dst'], env.OPTION_NONE)
    return 0
