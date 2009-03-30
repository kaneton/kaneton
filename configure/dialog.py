#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/configure/dialog.py
#
# created       julien quintard   [wed may 23 10:36:51 2007]
# updated       julien quintard   [sun mar 29 00:37:50 2009]
#

#
# ---------- information ------------------------------------------------------
#
# this module provides a wrapper upon the well-know dialog software.
#

#
# ---------- imports ----------------------------------------------------------
#

import env

#
# ---------- globals ----------------------------------------------------------
#

WINDOW_WIDTH=68
WINDOW_HEIGHT=19

MENU_HEIGHT=7

#
# ---------- functions --------------------------------------------------------
#

#
# menu()
#
# this function displays a menu.
#
def			menu(title, description, entries):
  choice = None
  height = None
  menu = ""

  for i in range(len(entries)):
    menu += "\"" + entries[i] + "\"" + " " + "\"\""

    if (i + 1) < len(entries):
      menu += " "

  description="\n" + description + "\n "

  env.launch("dialog", "--title \"" + title + "\" " +			\
                       "--menu \"" + description + "\" " +		\
                       str(WINDOW_HEIGHT) + " " + str(WINDOW_WIDTH) +	\
                       " " + str(MENU_HEIGHT) + " " + menu + " 2> " +	\
                       g_temporary, env.OPTION_NONE)

  choice = env.pull(g_temporary, env.OPTION_NONE)

  if not choice:
    return None

  return entries.index(choice)



#
# radio()
#
# this function displays a radio list.
#
def			radio(title, description, entries, current):
  choice = None
  menu = ""

  for i in range(len(entries)):
    if current == entries[i]:
      status = "\"on\""
    else:
      status = "\"off\""

    menu += "\"" + entries[i] + "\"" + " " + "\"\"" + " " + status

    if (i + 1) < len(entries):
      menu += " "

  description="\n" + description + "\n "

  env.launch("dialog", "--title \"" + title + "\" " +			\
                       " --radiolist \"" + description + "\" " +	\
                       str(WINDOW_HEIGHT) + " " + str(WINDOW_WIDTH) +	\
                       " " + str(MENU_HEIGHT) + " " + menu + " 2> " +	\
                       g_temporary, env.OPTION_NONE)

  choice = env.pull(g_temporary, env.OPTION_NONE)

  if not choice:
    return None

  return entries.index(choice)



#
# input()
#
# this function displays an input box.
#
def			input(title, description, current):
  choice = None

  description="\n" + description + "\n "

  env.launch("dialog", "--title \"" + title + "\" " +			\
                       " --inputbox \"" + description + "\" " +		\
                       str(WINDOW_HEIGHT) + " " + str(WINDOW_WIDTH) +	\
                       " " + current + " 2> " +	g_temporary,
                       env.OPTION_NONE)

  choice = env.pull(g_temporary, env.OPTION_NONE)

  return choice



#
# init()
#
# this function iniitialises the dialog package by building a temporary
# file.
#
def			init():
  global g_temporary

  g_temporary = env.temporary(env.OPTION_FILE)
