#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /Users/francois/kaneton/tool/export/modules/remove.py
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
    return ['tarball','filename']

#
# module_action(export_dir, arg)
#
# called by export.py when required by the YAML file.
# this function does the job of the action.
# arg contains the parameters required as specified in the module_init return
# value.
#
def module_action(export_dir, arg):
    env.display(env.HEADER_OK, 'action tarball ' + arg['filename'], env.OPTION_NONE)
    directory = env.cwd(env.OPTION_NONE)
    env.cd(env.path(export_dir, env.OPTION_DIRECTORY), env.OPTION_NONE)
    env.pack("kaneton", env._EXPORT_DIR_ + "/output/" + arg['filename'] + ".tar.bz2", env.OPTION_NONE)
    env.cd(directory, env.OPTION_NONE)
    return 0
