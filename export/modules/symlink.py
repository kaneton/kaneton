#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /Users/francois/kaneton/tool/export/modules/symlink.py
#
# created       francois goudal   [sat oct 25 20:57:38 2008]
# updated       francois goudal   [wed nov 12 05:26:29 2008]
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
    return ['symlink','name','target']

#
# module_action(export_dir, arg)
#
# called by export.py when required by the YAML file.
# this function does the job of the action.
# arg contains the parameters required as specified in the module_init return
# value.
#
def module_action(export_dir, arg):
    env.display(env.HEADER_OK, 'action symlink ' + arg['name'] + ' to ' + arg['target'], env.OPTION_NONE)
    env.link(export_dir + '/' + arg['name'], arg['target'], env.OPTION_NONE)
    return 0
