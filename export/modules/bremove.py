#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/export/modules/bremove.py
#
# created       francois goudal   [sat oct 25 20:57:38 2008]
# updated       julien quintard   [sat feb  5 10:33:21 2011]
#

#
# ---------- imports ----------------------------------------------------------
#

import env
import re

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
    return ['bremove','id']

#
# module_action(export_dir, arg)
#
# called by export.py when required by the YAML file.
# this function does the job of the action.
# arg contains the parameters required as specified in the module_init return
# value.
#
def module_action(export_dir, arg):
    strip = 0
    env.display(env.HEADER_OK, 'action bremove ' + arg['id'], env.OPTION_NONE)
    id = arg['id'].split('::', 1)
    filepath = id[0]
    label = id[1]
    tmpfilepath = env.temporary(env.OPTION_FILE)
    srcf = open(export_dir + '/' + filepath, 'r')
    dstf = open(tmpfilepath, 'w')
    line = srcf.readline()
    startpattern = re.compile(".*[[]block\:\:" + label + "[]].*")
    endpattern = re.compile(".*[[]/block\:\:" + label + "[]].*")
    while line != "":
        if strip == 0:
            if startpattern.match(line) != None:
                strip = 1
        if strip != 1:
            dstf.write(line)
        if strip == 1:
            if endpattern.match(line) != None:
                strip = 2
        line = srcf.readline()
    dstf.close()
    srcf.close()
    env.copy(tmpfilepath, export_dir + '/' + filepath, env.OPTION_NONE)
    env.remove(tmpfilepath, env.OPTION_NONE)
    if strip == 0:
        env.display(env.HEADER_ERROR, 'no block ' + label + ' in file ' + filepath, env.OPTION_NONE)
        return -1
    return 0
