#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/export/export.py
#
# created       francois goudal   [sat oct 25 20:57:38 2008]
# updated       julien quintard   [sat feb  5 11:43:43 2011]
#

#
# ---------- imports ----------------------------------------------------------
#

import sys
import yaml
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
# modules_load()
#
# dynamically loads action modules from the "modules" directory.
#
def modules_load():
  global g_modules

  env.display(env.HEADER_OK, 'loading action modules', env.OPTION_NONE)
  if (env.path(sys.argv[0], env.OPTION_DIRECTORY) == ""):
    moduledir = './modules'
  else:
    moduledir = env.path(sys.argv[0], env.OPTION_DIRECTORY) + '/modules'
  sys.path.append(moduledir)
  for i in env.list(moduledir, env.OPTION_FILE):
    if i.endswith('.py'):
      mod = __import__(i[:-3])
      try:
        init_str = mod.module_init()
      except AttributeError:
        sys.stderr.write('error: The module file ' + i +
                         ' is missing the module_init function\n')
        sys.exit(1)
      g_modules[init_str[0]] = mod
      g_modules_parameters[init_str[0]] = init_str
      env.display(env.HEADER_OK, "  loaded module " + init_str[0], env.OPTION_NONE)

#
# action_data_load()
#
# loads the action data from the YAML file
#
def action_data_load(filename):
  env.display(env.HEADER_OK, 'loading action data description file ' + filename, env.OPTION_NONE)
  stream = file(env._EXPORT_DIR_ + '/' + filename + '.yml', 'r')
  return yaml.load(stream)



def parse_data_file(filename):
  global g_action_data
  global g_modules
  global g_export_dir
  global g_modules_parameters

  g_action_data = action_data_load(filename)
  for i in g_action_data:
    if i['operation'] == 'import':
      env.display(env.HEADER_OK, 'action import ' + i['filename'], env.OPTION_NONE)
      parse_data_file("behaviours/" + i['filename'])
    else:   
      arg = {}
      for j in range(1, len(g_modules_parameters[i['operation']])):
        arg[g_modules_parameters[i['operation']][j]] = i[g_modules_parameters[
            i['operation']][j]];
      if g_modules[i['operation']].module_action(g_export_dir, arg) != 0:
        sys.exit(1)


#
# main()
#
# main function: goes through the actions and calls the modules accordingly
#
def main():
  global g_modules
  global g_action_data
  global g_export_dir

  if (len(sys.argv) != 2):
    env.display(env.HEADER_ERROR, "usage: export.py [behaviour]",
                env.OPTION_NONE)
    env.display(env.HEADER_NONE, "",
                env.OPTION_NONE)
    env.display(env.HEADER_ERROR, "behaviours:",
                env.OPTION_NONE)
    for i in env.list(env._EXPORT_DIR_ + "/behaviours", env.OPTION_FILE):
      env.display(env.HEADER_ERROR, "  " + i[:-4], env.OPTION_NONE)
    return 1

  modules_load()

  work_dir = env.temporary(env.OPTION_DIRECTORY)

  g_export_dir =  work_dir + "/kaneton"

  env.display(env.HEADER_OK, 'exporting to ' + g_export_dir, env.OPTION_NONE)

  env.display(env.HEADER_OK, 'running export actions', env.OPTION_NONE)

  parse_data_file("behaviours/" + sys.argv[1])

  env.display(env.HEADER_OK, 'cleaning temporary directory', env.OPTION_NONE)
  env.remove(work_dir, env.OPTION_NONE)

  env.display(env.HEADER_OK, 'export finished', env.OPTION_NONE)
  
#
# ---------- entry point ------------------------------------------------------
#

main()
