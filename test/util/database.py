#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/util/database.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [wed oct 20 13:52:45 2010]
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

g_component = None
g_path = None

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
# student()
#
# this function generates a database for the given student.
#
def                     student():
  school = None
  array = None
  id = None

  # display.
  env.display(env.HEADER_OK,
              "generating database for the student",
              env.OPTION_NONE)

  # compute the id.
  id = g_path.replace("/", "::")

  # compute the school.
  array = g_path.split("/")
  school = "::".join(array[:len(array) - 1])

  # load the configuration.
  configuration = yaml.load(file(env._TEST_CONFIGURATION_DIR_ +         \
                                   "/" + school + ".conf", 'r'))

  # store the database in the file.
  yaml.dump("{environments: " + configuration + ", traces: None }",
            file(env._TEST_STORE_DATABASE_DIR_ + "/" + id + ".db", 'w'))

  # display.
  env.display(env.HEADER_OK,
              "  " +
              id,
              env.OPTION_NONE)

  # display message.
  env.display(env.HEADER_OK,
              "student's database generated and stored",
              env.OPTION_NONE)

#
# school()
#
# this function generates a database for all the students of the given school
# and according to the configuration file located in test/configuration/.
#
def                     school():
  students = None
  student = None
  id = None

  # display.
  env.display(env.HEADER_OK,
              "generating databases for the students",
              env.OPTION_NONE)

  # retrieve the list of students.
  students = env.list(env._HISTORY_DIR_ + "/" + g_path,
                      env.OPTION_DIRECTORY)

  # for each student.
  for student in students:
    # compute the id.
    id = g_path.replace("/", "::")

    # load the configuration.
    configuration = yaml.load(file(env._TEST_CONFIGURATION_DIR_ +       \
                                     "/" + id + ".conf", 'r'))

    # store the database in the file.
    yaml.dump("{environments: " + configuration + ", traces: None }",
              file(env._TEST_STORE_DATABASE_DIR_ + "/" + id + "::" +    \
                     student + ".db", 'w'))

    # display.
    env.display(env.HEADER_OK,
                "  " +
                id + "::" + student,
                env.OPTION_NONE)

  # display message.
  env.display(env.HEADER_OK,
              "students' database generated and stored",
              env.OPTION_NONE)


#
# contributor()
#
# this function creates a database for the contributors.
#
def                     contributor():
  configuration = None

  # display message.
  env.display(env.HEADER_OK,
              "generating database from contributor's configuration",
              env.OPTION_NONE)

  # load the configuration.
  configuration = yaml.load(file(env._TEST_CONFIGURATION_DIR_ +         \
                            "/contributor.conf", 'r'))

  # store the database in the file.
  yaml.dump("{environments: " + configuration + ", traces: None }",
            file(env._TEST_STORE_DATABASE_DIR_ + "/contributor.db", 'w'))

  # display message.
  env.display(env.HEADER_OK,
              "contributor's database generated and stored",
              env.OPTION_NONE)

#
# main()
#
# this is the main function.
#
def                     main():
  global g_component
  global g_path

  component = None

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  # retrieve the arguments.
  g_component = sys.argv[1].split("@")[0]

  if len(sys.argv[1].split("@")) > 1:
    g_path = sys.argv[1].split("@")[1].replace("::", "/")

  # trigger the appropriate function.
  for component in c_components:
    if g_component == component.split("@")[0]:
      c_components[component]()
      sys.exit(0)

  # wrong component.
  env.display(env.HEADER_ERROR,
              "unknown component '" + g_component + "'",
              env.OPTION_NONE)

  # display usage.
  Usage()

#
# ---------- constants --------------------------------------------------------
#

c_components = {
  "contributor": contributor,
  "school@school::year": school,
  "student@school::year::name": student
}

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
