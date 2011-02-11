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
# updated       julien quintard   [fri feb 11 10:50:39 2011]
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
# student()
#
# this function generates a database for the members of the given group.
#
def                     student():
  components = None
  school = None
  year = None
  group = None
  capability = None
  name = None
  path = None
  configuration = None
  database = None
  member = None

  # display.
  env.display(env.HEADER_OK,
              "generating database for the students of the group '" +   \
                g_path + "'",
              env.OPTION_NONE)

  # retrieve the components.
  components = g_path.strip("/").split("/")

  # compute the school, year and group.
  school = components[0]
  year = components[1]
  group = components[2]

  # compute the name.
  name = school + "::" + year + "::" + group

  # load the group capability.
  capability = ktp.capability.Load(env._TEST_STORE_CAPABILITY_DIR_ +  \
                                     "/" + name + ktp.capability.Extension)

  # go through the group members.
  for member in capability["members"]:
    # display message.
    env.display(env.HEADER_OK,
                "  " + member["login"],
                env.OPTION_NONE)

    # compute the name.
    name = school + "::" + year + "::" + group + "::" + member["login"]

    # compute the file name.
    path = env._TEST_STORE_DATABASE_DIR_ + "/" + name + ktp.database.Extension

    # load the configuration.
    configuration = ktp.configuration.Load(env._TEST_CONFIGURATION_DIR_ + \
                                             "/" + school + "::" +        \
                                             year + "#student" +          \
                                             ktp.configuration.Extension)

    # generate the database.
    database = ktp.database.Generate(configuration)

    # store the database.
    ktp.database.Store(database, path)

  # display message.
  env.display(env.HEADER_OK,
              "the group members' databases have been generated succesfully",
              env.OPTION_NONE)

#
# group()
#
# this function generates a database for the given group.
#
def                     group():
  components = None
  school = None
  year = None
  group = None
  name = None
  path = None
  configuration = None
  database = None

  # display.
  env.display(env.HEADER_OK,
              "generating database for the group '" + g_path + "'",
              env.OPTION_NONE)

  # retrieve the components.
  components = g_path.strip("/").split("/")

  # compute the school, year and group.
  school = components[0]
  year = components[1]
  group = components[2]

  # compute the name.
  name = school + "::" + year + "::" + group

  # compute the file name.
  path = env._TEST_STORE_DATABASE_DIR_ + "/" + name + ktp.database.Extension

  # load the configuration.
  configuration = ktp.configuration.Load(env._TEST_CONFIGURATION_DIR_ + \
                                           "/" + school + "::" +        \
                                           year + "#group" +            \
                                           ktp.configuration.Extension)

  # generate the database.
  database = ktp.database.Generate(configuration)

  # store the database.
  ktp.database.Store(database, path)

  # display message.
  env.display(env.HEADER_OK,
              "the group database '" + name + "' has been " +           \
                "generated succesfully",
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
  school = None
  group = None
  name = None
  configuration = None
  name = None
  capability = None
  database = None

  # display.
  env.display(env.HEADER_OK,
              "generating students' databases for the school '" +       \
                g_path + "'",
              env.OPTION_NONE)

  # retrieve the list of students.
  students = env.list(env._HISTORY_DIR_ + "/" + g_path,
                      env.OPTION_DIRECTORY)

  # for each student.
  for student in students:
    # display.
    env.display(env.HEADER_OK,
                "  " + student,
                env.OPTION_NONE)

    # retrieve the components.
    components = g_path.strip("/").split("/")

    # compute the school, year and group.
    school = components[0]
    year = components[1]
    group = student

    # compute the group capability/database name.
    name = school + "::" + year + "::" + group

    # compute the file name.
    path = env._TEST_STORE_DATABASE_DIR_ + "/" + name + ktp.database.Extension

    # load the group capability.
    capability = ktp.capability.Load(env._TEST_STORE_CAPABILITY_DIR_ +  \
                                       "/" + name + ktp.capability.Extension)

    # load the configuration.
    configuration = ktp.configuration.Load(env._TEST_CONFIGURATION_DIR_ + \
                                             "/" + school + "::" +        \
                                             year + "#group" +            \
                                             ktp.configuration.Extension)

    # generate the database.
    database = ktp.database.Generate(configuration)

    # store the database.
    ktp.database.Store(database, path)

    # go through the list of members attached to this group.
    for member in capability["members"]:
      # compute the name.
      name = school + "::" + year + "::" + group + "::" + member["login"]

      # compute the file name.
      path = env._TEST_STORE_DATABASE_DIR_ + "/" +                      \
        name + ktp.database.Extension

      # load the configuration.
      configuration = ktp.configuration.Load(env._TEST_CONFIGURATION_DIR_ + \
                                               "/" + school + "::" +        \
                                               year + "#student" +          \
                                               ktp.configuration.Extension)

      # generate the database.
      database = ktp.database.Generate(configuration)

      # store the database.
      ktp.database.Store(database, path)

  # display message.
  env.display(env.HEADER_OK,
              "the school's group capabilities have been generated " +  \
                "succesfully",
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
              "generating database for the contributor",
              env.OPTION_NONE)

  # load the configuration.
  configuration = ktp.configuration.Load(env._TEST_CONFIGURATION_DIR_ + \
                                           "/contributor" +             \
                                           ktp.configuration.Extension)

  # generate the database.
  database = ktp.database.Generate(configuration)

  # store the database.
  ktp.database.Store(database,
                     env._TEST_STORE_DATABASE_DIR_ + "/" + env._USER_ + \
                       ktp.database.Extension)

  # display message.
  env.display(env.HEADER_OK,
              "the contributor database has been generated for '" +     \
                env._USER_ + "'",
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
              "generating database for the robot",
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
              "the robot's database has been generated succesfully",
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
  "group@school::year::group": group,
  "student@school::year::group": student
}

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
