#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /Users/francois/kaneton/tool/export/export.py
#
# created       francois goudal   [sat oct 25 20:57:38 2008]
# updated       francois goudal   [wed nov 12 05:26:29 2008]
#

#
# ---------- imports ----------------------------------------------------------
#

import os
import sys
import yaml
import tempfile
import env

#
# ---------- globals ----------------------------------------------------------
#

g_verbose = 1
g_modules = {}
g_modules_parameters = {}
g_action_data = []
g_export_dir = ""

#
# ---------- functions --------------------------------------------------------
#


#
# myprint(str)
#
# overlay to the print statement that only prints str if g_verbose is set
#
def myprint(str):
  global g_verbose

  if (g_verbose != 0):
    print str

#
# modules_load()
#
# dynamically loads action modules from the "modules" directory.
#
def modules_load():
  global g_modules

  if (os.path.dirname(sys.argv[0]) == ""):
    moduledir = './modules'
  else:
    moduledir = os.path.dirname(sys.argv[0]) + '/modules'
  sys.path.append(moduledir)
  for i in os.listdir(moduledir):
    if os.path.splitext(i)[1] == '.py':
      mod = __import__(os.path.splitext(i)[0])
      try:
        init_str = mod.module_init()
      except AttributeError:
        sys.stderr.write('error: The module file ' + i +
                         ' is missing the module_init function\n')
        sys.exit(1)
      g_modules[init_str[0]] = mod
      g_modules_parameters[init_str[0]] = init_str
      myprint("Loaded module " + init_str[0])



#
# action_data_load()
#
# loads the action data from the YAML file
#
def action_data_load():
  global g_action_data

  stream = file(env._EXPORT_DIR_ + '/' + sys.argv[1] + '.yml', 'r')
  g_action_data = yaml.load(stream)
  myprint(g_action_data)



#
# main()
#
# main function: goes through the actions and calls the modules accordingly
#
def main():
  global g_modules
  global g_action_data
  global g_export_dir

  g_export_dir = tempfile.mkdtemp('','kaneton-export_', '/tmp')
  myprint('Copying the source tree to ' + g_export_dir + ' ...')
  env.launch(env._CP_, "-r " + env._SOURCE_DIR_ + "/* " +
             g_export_dir, env.OPTION_QUIET)
  myprint('Done.')

  myprint('Loading action modules...')
  modules_load()
  myprint('Done.')

  myprint('Loading action data description file...')
  action_data_load()
  myprint('Done.')

  for i in g_action_data:
    arg = {}
    for j in range(1, len(g_modules_parameters[i['operation']])):
      arg[g_modules_parameters[i['operation']][j]] = i[g_modules_parameters[
          i['operation']][j]];
    g_modules[i['operation']].module_action(g_export_dir, arg)



#
# ---------- entry point ------------------------------------------------------
#

main()
