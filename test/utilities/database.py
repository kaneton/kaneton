#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/utilities/database.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [fri feb  4 21:32:31 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this program generates the database file.
#

#
# ---------- packages ---------------------------------------------------------
#

import sys
import re
import yaml
import copy

import env
import ktp

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
  component = None

  env.display(env.HEADER_ERROR,
              "usage: database.py [component]",
              env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  env.display(env.HEADER_ERROR, "components:", env.OPTION_NONE)

  for component in c_components:
    env.display(env.HEADER_ERROR, "  " + component, env.OPTION_NONE)

#
# personal()
#
# this function generates a personal database.
#
def                     personal(school,
                                 id):
  members = None
  member = None

  # load the group capability.
  capability = ktp.capability.Load(env._TEST_STORE_CAPABILITY_DIR_ +    \
                                     "/" + id + ktp.capability.Extension)

  # display.
  env.display(env.HEADER_OK,
              "generating database for the group's student members",
              env.OPTION_NONE)

  # retrieve the members.
  members = capability["members"]

  # go through the members.
  for member in members:
    # generate a name.
    name = member["name"].lower().replace(" ", ".")

    # load the configuration.
    configuration = ktp.configuration.Load(env._TEST_CONFIGURATION_DIR_ +\
                                             "/" + school + "#student" + \
                                             ktp.configuration.Extension)

    # generate the database.
    database = ktp.database.Generate(configuration)

    # store the database.
    ktp.database.Store(database,
                       env._TEST_STORE_DATABASE_DIR_ + "/" +            \
                         name + ktp.database.Extension)

    # display.
    env.display(env.HEADER_OK,
                "  " + name,
                env.OPTION_NONE)

  # display.
  env.display(env.HEADER_OK,
              "databases for the group's student members generated",
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
  database = None
  capability = None

  # display.
  env.display(env.HEADER_OK,
              "generating database for the student group",
              env.OPTION_NONE)

  # compute the id.
  id = g_path.replace("/", "::")

  # compute the school.
  array = g_path.split("/")
  school = "::".join(array[:len(array) - 1])

  # load the configuration.
  configuration = ktp.configuration.Load(env._TEST_CONFIGURATION_DIR_ + \
                                           "/" + school + "#group" +    \
                                           ktp.configuration.Extension)

  # generate the database.
  database = ktp.database.Generate(configuration)

  # store the database.
  ktp.database.Store(database, env._TEST_STORE_DATABASE_DIR_ + "/" +    \
                       id + ktp.database.Extension)

  # display.
  env.display(env.HEADER_OK,
              "  " +
              id,
              env.OPTION_NONE)

  # display message.
  env.display(env.HEADER_OK,
              "student's group database generated and stored",
              env.OPTION_NONE)

  # generate the personal databases.
  personal(school, id)

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
  configuration = None
  database = None
  name = None

  # display.
  env.display(env.HEADER_OK,
              "generating databases for the students",
              env.OPTION_NONE)

  # retrieve the list of students.
  students = env.list(env._HISTORY_DIR_ + "/" + g_path,
                      env.OPTION_DIRECTORY)

  # for each student.
  for student in students:
    # compute the school id.
    id = g_path.replace("/", "::")

    # compute the student name.
    name = id + "::" + student

    # load the configuration.
    configuration = ktp.configuration.Load(env._TEST_CONFIGURATION_DIR_ + \
                                             "/" + id + "#group" +        \
                                             ktp.configuration.Extension)

    # generate the database.
    database = ktp.database.Generate(configuration)

    # store the database.
    ktp.database.Store(database, env._TEST_STORE_DATABASE_DIR_ + "/" +  \
                         name + ktp.database.Extension)

    # display.
    env.display(env.HEADER_OK,
                "  " +
                name,
                env.OPTION_NONE)

    # generate the personal databases.
    personal(id, name)

  # display message.
  env.display(env.HEADER_OK,
              "students' database generated and stored",
              env.OPTION_NONE)

#
# contributor()
#
# this function creates a database for the contributor.
#
def                     contributor():
  configuration = None
  database = None

  # display message.
  env.display(env.HEADER_OK,
              "generating database from contributor's configuration",
              env.OPTION_NONE)

  # load the configuration.
  configuration = ktp.configuration.Load(env._TEST_CONFIGURATION_DIR_ + \
                                           "/contributor" +             \
                                           ktp.configuration.Extension)

  # generate the database.
  database = ktp.database.Generate(configuration)

  # store the database.
  ktp.database.Store(database,
                     env._TEST_STORE_DATABASE_DIR_ + "/contributor" +   \
                       ktp.database.Extension)

  # display message.
  env.display(env.HEADER_OK,
              "contributor's database generated and stored",
              env.OPTION_NONE)

#
# robot()
#
# this function creates a database for the robot.
#
def                     robot():
  configuration = None
  database = None

  # display message.
  env.display(env.HEADER_OK,
              "generating database from robot's configuration",
              env.OPTION_NONE)

  # load the configuration.
  configuration = ktp.configuration.Load(env._TEST_CONFIGURATION_DIR_ + \
                                           "/robot" +                   \
                                           ktp.configuration.Extension)

  # generate the database.
  database = ktp.database.Generate(configuration)

  # store the database.
  ktp.database.Store(database,
                     env._TEST_STORE_DATABASE_DIR_ + "/robot" +         \
                       ktp.database.Extension)

  # display message.
  env.display(env.HEADER_OK,
              "robot's database generated and stored",
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
  "robot": robot,
  "school@school::year": school,
  "student@school::year::name": student
}

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
