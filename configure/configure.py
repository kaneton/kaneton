#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/configure/configure.py
#
# created       julien quintard   [wed may 23 10:17:59 2007]
# updated       julien quintard   [tue may 29 18:36:47 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this script is a user-friendly software allowing users to customize
# their own development environment
#

#
# ---------- imports ----------------------------------------------------------
#

import critical

import env
import yaml
import sys

import environment
import dialog

#
# ---------- types ------------------------------------------------------------
#

class c_entry:
  def __init__(self):
    self.type = None

    self.frame = None
    self.path = None

    self.variable = None

class c_frame:
  def __init__(self):
    self.title = None
    self.description = None

    self.entries = []

    # a list of strings rather that a list of objects: self.entries
    self.list = []

#
# ---------- globals ----------------------------------------------------------
#

g_frames = []
g_frame = None

g_path = None

#
# ---------- options ----------------------------------------------------------
#

TYPE_FRAME = 1
TYPE_VARIABLE = 2

#
# ---------- functions --------------------------------------------------------
#

#
# load()
#
# this function loads the current frame into a instance of the
# frame class.
#
def			load(path):
  global g_frames
  global g_frame
  streams = None
  stream = None
  object = None
  entry = None

  # load the YAML entries.
  streams = yaml.load(env.pull(path, env.OPTION_READ))

  # create an new frame object.
  g_frame = c_frame()

  # try to fill the g_frame object with the YAML entries.
  for stream in streams:
    try:
      g_frame.title = stream["title"]
      g_frame.description = stream["description"]

      continue
    except:
      pass

    entry = c_entry()

    try:
      entry.frame = stream["frame"]
      entry.path = stream["path"]
      entry.type = TYPE_FRAME

      g_frame.list.append(entry.frame)
      g_frame.entries.append(entry)
    except:
      try:
        entry.type = TYPE_VARIABLE
        entry.variable = environment.get(stream["variable"])

        if not entry.variable:
          continue

        g_frame.list.append(entry.variable.string)
        g_frame.entries.append(entry)
      except:
        continue

  # finally add the frame to the list of frames.
  g_frames.append(g_frame)



#
# radio()
#
# this function displays a radiolist in order to ask the user to
# choice a value among a list.
#
def			radio(object):
  default = None
  value = None
  key = None
  menu = []

  for value in object.values:
    key = value.keys()[0]

    if object.value == value[key]:
      default = key

    menu.append(key)

  dialog.radio(g_title, menu, default)



#
# variable()
#
# this function dispatch the function call to an appropriate function
# depending on the variable value type.
#
def			variable(entry):
  object = None
  value = None

  object = environment.get(entry.variable)

  if object.type == environment.TYPE_STATE:
    value = check(object)
  elif object.type == environment.TYPE_SET:
    value = radio(object)
  elif object.type == environment.TYPE_ANY:
    value = input(object)



#
# main()
#
# this function starts by loading the environment and then displays the
# interfactive frames.
#
def			main():
  global g_path
  choice = None

  # load the environment variables.
  environment.init()

  # initialiase the dialog package.
  dialog.init()

  # initialise the 'g_path' variable.
  g_path = "configure.frm"

  # main loop until there is a frame or variable to display.
  while True:
    # load the information on the current frame.
    load(g_path)

    print g_frame.list

    # display the current frame and get the user's choice.
#    choice = draw_frame()

    sys.exit(1)

    # did the user want to go back?
#    if not entry:
#      g_frames.pop(len(g_frames) - 1)

#      if len(g_frames) == 0:
#        sys.exit(0)

#      continue

    # if the choice is a subframe, then go to it. otherwise, ask the
    # user to enter a new value for the variable.
#    if entry.type == TYPE_FRAME:
#      g_frames.append(entry.path)

#    elif entry.type == TYPE_VARIABLE:
#      variable(entry)

#
# ---------- entry point ------------------------------------------------------
#

main()
