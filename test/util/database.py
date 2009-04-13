#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/util/database.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [sun apr 12 18:03:50 2009]
#

#
# ---------- information ------------------------------------------------------
#
# this program generates the database file.
#

#
# ---------- packages ---------------------------------------------------------
#

import env
import ktc

import sys
import re
import yaml
import copy

#
# ---------- globals ----------------------------------------------------------
#

g_school = None
g_year = None

g_stages = "data/stages.yml"

#
# ---------- functions --------------------------------------------------------
#

#
# usage()
#
# this function displays the usage.
#
def			usage():
  env.display(env.HEADER_ERROR,
              "usage: database.py [school::year]",
              env.OPTION_NONE)

#
# warning()
#
# this function asks the user the permission to continue.
#
def			warning():
  env.display(env.HEADER_INTERACTIVE,
              "to cancel press CTRL^C, otherwise press enter",
              env.OPTION_NONE)

  env.input(env.OPTION_NONE)

#
# extract()
#
# this function takes the path to a history directory and
# extract the names, emails and id of every group.
#
# finally, the function returns the information in a data
# structure.
#
def                     extract(path):
  students = []
  groups = None
  group = None
  members = None
  member = None
  name = None
  email = None

  # display.
  env.display(env.HEADER_OK,
              "extracting the students from the history '" + path + "'",
              env.OPTION_NONE)

  # retrieve the list of groups.
  groups = env.list(env._HISTORY_DIR_ + "/" + path,
                    env.OPTION_DIRECTORY)

  # for each group, extract the information from the
  # 'people' file.
  for group in groups:
    # read the 'people' file.
    people = env.pull(env._HISTORY_DIR_ + "/" + path + "/" +
                      group + "/people", env.OPTION_NONE)

    # extract every line
    members = people.strip("\n").split("\n")

    # for every member, extract name and email.
    for member in members:
      # apply a regexp to locate the elements.
      match = re.match("^(.+) <(.+)>$", member)

      if not match:
        env.display(env.HEADER_ERROR,
                    "  unable to extract the name and email from the group '" +
                    group + "'",
                    env.OPTION_NONE)

      # extract name and email.
      name = match.group(1)
      email = match.group(2)

      # add the member to the data structure.
      students += [ { "group": group,
                      "name": name,
                      "email": email } ]

  # display.
  env.display(env.HEADER_OK,
              "students information retrieved",
              env.OPTION_NONE)

  return students

#
# generate()
#
# this function takes a data structure and generate
# the database by initialising the number of tests
# a student will be allowed to launch per stage.
#
def                     generate(students, name):
  student = None
  database = {}

  # display.
  env.display(env.HEADER_OK,
              "generating the students records",
              env.OPTION_NONE)

  # load the stages information.
  stages = yaml.load(file(g_stages, 'r'))

  # for every student, generate a capability and store it.
  for student in students:
    # compute the id.
    id = student["name"].replace(" ", ".").lower()

    # record the information in the database.
    database[id] = copy.copy(stages);

    # store the database in the file.
    yaml.dump(database, file(env._TEST_STORE_DATABASE_DIR_ + "/" + id + ".db", 'w'))

#
# main()
#
# this is the main function.
#
def                     main():
  global g_school
  global g_year

  file = None
  code = None
  students = None
  args = None
  path = None

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  # retrive the actual arguments.
  args = sys.argv[1].split("::")

  # check the number of arguments.
  if len(args) != 2:
    usage()
    sys.exit(42)

  # set the global variables.
  g_school = args[0]
  g_year = args[1]

  # build the path.
  path = g_school + "/" + g_year

  # display.
  env.display(env.HEADER_OK,
              "generating database",
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # display a warning.
  warning()

  # read the file and extract the students information.
  students = extract(path)

  # generate the database.
  generate(students, sys.argv[1])

  # display.
  env.display(env.HEADER_OK,
              "databases generated successfully",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
