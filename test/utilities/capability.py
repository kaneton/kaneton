#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/utilities/capability.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [sat feb 19 23:54:42 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this program generates capabilities for every student.
#

#
# ---------- packages ---------------------------------------------------------
#

import sys
import re

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
              "usage: capability.py [component]",
              env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  env.display(env.HEADER_ERROR, "components:", env.OPTION_NONE)

  for component in c_components:
    env.display(env.HEADER_ERROR, "  " + component, env.OPTION_NONE)

#
# student()
#
# this function generates the capabilities for the group's members.
#
def                     student():
  code = None
  components = None
  school = None
  year = None
  group = None
  name = None
  people = None
  members = None
  member = None
  path = None
  capability = None

  # display.
  env.display(env.HEADER_OK,
              "generating a capability for the group '" + g_path + "'",
              env.OPTION_NONE)

  # retrieve the server's code.
  code = ktp.code.Load(env._TEST_STORE_CODE_DIR_ + "/server" +          \
                         ktp.code.Extension)

  # compute the school, year, group and identifier name.
  components = g_path.strip("/").split("/")

  # compute the school, year and group.
  school = components[0]
  year = components[1]
  group = components[2]

  # read the 'people' file.
  people = env.pull(env._HISTORY_DIR_ + "/" + g_path + "/people",
                    env.OPTION_NONE).strip("\n").split("\n")

  # initialize the members list.
  members = []

  # for every member, extract name and email.
  for member in people:
    # apply a regexp to locate the elements.
    match = re.match("^(.+) <(.+)>$", member)

    if not match:
      env.display(env.HEADER_ERROR,
                  "  unable to extract the name and email from the student '" +
                  g_path + "'",
                  env.OPTION_NONE)
      continue
    else:
      # remember the member.
      member = { "name": match.group(1),
                 "email": match.group(2),
                 "login": match.group(1).lower().replace(" ", ".") }

    # display.
    env.display(env.HEADER_OK,
                "  " + member["login"],
                env.OPTION_NONE)

    # compute the unique name.
    name = school + "::" + year + "::" + group + "::" + member["login"]

    # compute the file name.
    path = env._TEST_STORE_CAPABILITY_DIR_ + "/" +                      \
      name + ktp.capability.Extension

    # create the capability.
    capability = ktp.capability.Create(code,
                                       name,
                                       ktp.capability.TypeStudent,
                                       { "school": school,
                                         "year": year,
                                         "group": group,
                                         "student": member["login"] },
                                       [ member ])

    # store it.
    ktp.capability.Store(path,
                         capability)

  # display.
  env.display(env.HEADER_OK,
              "the group member capabilities have been generated succesfully",
              env.OPTION_NONE)

#
# group()
#
# this function generates a capability for a specific group.
#
def                     group():
  code = None
  components = None
  school = None
  year = None
  group = None
  name = None
  people = None
  members = None
  member = None
  path = None
  capability = None

  # display.
  env.display(env.HEADER_OK,
              "generating a capability for the group '" + g_path + "'",
              env.OPTION_NONE)

  # retrieve the server's code.
  code = ktp.code.Load(env._TEST_STORE_CODE_DIR_ + "/server" +          \
                         ktp.code.Extension)

  # compute the school, year, group and identifier name.
  components = g_path.strip("/").split("/")

  # compute the school, year, group and unique name.
  school = components[0]
  year = components[1]
  group = components[2]
  name = school + "::" + year + "::" + group

  # read the 'people' file.
  people = env.pull(env._HISTORY_DIR_ + "/" + g_path + "/people",
                    env.OPTION_NONE).strip("\n").split("\n")

  # initialize the members list.
  members = []

  # for every member, extract name and email.
  for member in people:
    # apply a regexp to locate the elements.
    match = re.match("^(.+) <(.+)>$", member)

    if not match:
      env.display(env.HEADER_ERROR,
                  "  unable to extract the name and email from the student '" +
                  g_path + "'",
                  env.OPTION_NONE)
      continue
    else:
      # register the member.
      members += [ { "name": match.group(1),
                     "email": match.group(2),
                     "login": match.group(1).lower().replace(" ", ".") } ]

  # compute the file name.
  path = env._TEST_STORE_CAPABILITY_DIR_ + "/" +                        \
    name + ktp.capability.Extension

  # create the capability.
  capability = ktp.capability.Create(code,
                                     name,
                                     ktp.capability.TypeGroup,
                                     { "school": school,
                                       "year": year,
                                       "group": group },
                                     members)

  # store it.
  ktp.capability.Store(path,
                       capability)

  # display.
  env.display(env.HEADER_OK,
              "the group capability '" + name + "' has been " +         \
                "generated succesfully",
              env.OPTION_NONE)

#
# school()
#
# this function generates a capability for a set of students identified
# by their school and year.
#
def                     school():
  code = None
  students = None
  student = None
  people = None
  members = None
  member = None
  match = None
  components = None
  school = None
  year = None
  group = None
  name = None
  path = None
  capability = None

  # display.
  env.display(env.HEADER_OK,
              "generating students' capabilities for the school '" +    \
                g_path + "'",
              env.OPTION_NONE)

  # retrieve the server's code.
  code = ktp.code.Load(env._TEST_STORE_CODE_DIR_ + "/server" +          \
                         ktp.code.Extension)

  # extract the components from the path.
  components = g_path.strip("/").split("/")

  # compute the school and year.
  school = components[0]
  year = components[1]

  # retrieve the list of students.
  students = env.list(env._HISTORY_DIR_ + "/" + g_path,
                      env.OPTION_DIRECTORY)

  # for each student, extract the information from the
  # 'people' file.
  for student in students:
    # display.
    env.display(env.HEADER_OK,
                "  " + student,
                env.OPTION_NONE)

    # complete the attributes with the group name.
    group = student
    name = school + "::" + year + "::" + group

    # read the 'people' file.
    people = env.pull(env._HISTORY_DIR_ + "/" + g_path + "/" +
                      student + "/people",
                      env.OPTION_NONE).strip("\n").split("\n")

    # initialize the members list.
    members = []

    # for every member, extract the first email email.
    for member in people:
      # apply a regexp to locate the elements.
      match = re.match("^(.+) <(.+)>$", member)

      if not match:
        env.display(env.HEADER_ERROR,
                    "  unable to extract the name and email from the "
                    "student '" + student + "'",
                    env.OPTION_NONE)
        continue
      else:
        # register the member.
        members += [ { "name": match.group(1),
                       "email": match.group(2),
                       "login": match.group(1).lower().replace(" ", ".") } ]

    # compute the file name.
    path = env._TEST_STORE_CAPABILITY_DIR_ + "/" +                      \
      name + ktp.capability.Extension

    # create the capability.
    capability = ktp.capability.Create(code,
                                       name,
                                       ktp.capability.TypeGroup,
                                       { "school": school,
                                         "year": year,
                                         "group": group },
                                       members)

    # store it.
    ktp.capability.Store(path,
                         capability)

    # go through the list of members attached to this group.
    for member in members:
      # generate a student name.
      name = school+ "::" + year + "::" + group + "::" + member["login"]

      # compute the file name.
      path = env._TEST_STORE_CAPABILITY_DIR_ + "/" +                    \
        name + ktp.capability.Extension

      # create a specific capability for the member.
      capability = ktp.capability.Create(code,
                                         name,
                                         ktp.capability.TypeStudent,
                                         { "school": school,
                                           "year": year,
                                           "group": group,
                                           "student": member["login"] },
                                         [ { "name": member["name"],
                                             "email": member["email"],
                                             "login": member["login"] } ])

      # store it.
      ktp.capability.Store(path,
                           capability)

  # display.
  env.display(env.HEADER_OK,
              "the school student capabilities have been generated " +  \
                "succesfully",
              env.OPTION_NONE)

#
# contributor()
#
# this function generates a capability for the contributor to
# test the research implementation as many times as they wish.
#
def                     contributor():
  path = None
  link = None
  name = None

  # display.
  env.display(env.HEADER_OK,
              "generating a contributor's capability",
              env.OPTION_NONE)

  # retrieve the server's code.
  code = ktp.code.Load(env._TEST_STORE_CODE_DIR_ + "/server" +          \
                         ktp.code.Extension)

  # compute the name.
  name = env._USER_

  # compute the file name.
  path = env._TEST_STORE_CAPABILITY_DIR_ + "/" +                        \
    name + ktp.capability.Extension

  # compute the link.
  link = env._TEST_CAPABILITY_

  # create the capability.
  capability = ktp.capability.Create(code,
                                     name,
                                     ktp.capability.TypeContributor,
                                     {},
                                     [ { "name": env._TEST_NAME_,
                                         "email": env._TEST_EMAIL_,
                                         "login":
                                           env._TEST_NAME_.lower().
                                             replace(" ", ".") } ])

  # store it.
  ktp.capability.Store(path,
                       capability)

  # remove the previous link.
  env.remove(link, env.OPTION_NONE)

  # create a link from the profile. XXX[this could probably be improved]
  env.link(link,
           "../../../../test/store/capability/" + name +                
             ktp.capability.Extension,
           env.OPTION_NONE)

  # display.
  env.display(env.HEADER_OK,
              "the contributor capability has been generated " +        \
              "succesfully for '" + name + "'",
              env.OPTION_NONE)

#
# robot()
#
# this function generates a capability for the robot.
#
def                     robot():
  path = None
  link = None
  name = None

  # display.
  env.display(env.HEADER_OK,
              "generating the robot's capability",
              env.OPTION_NONE)

  # retrieve the server's code.
  code = ktp.code.Load(env._TEST_STORE_CODE_DIR_ + "/server" +          \
                         ktp.code.Extension)

  # compute the name.
  name = "robot"

  # compute the file name.
  path = env._TEST_STORE_CAPABILITY_DIR_ + "/" +                        \
    name + ktp.capability.Extension

  # compute the link.
  link = env._PROFILE_DIR_ + "/user/robot/robot" + ktp.capability.Extension

  # create the capability.
  capability = ktp.capability.Create(code,
                                     name,
                                     ktp.capability.TypeStudent,
                                     {},
                                     [ { "name":
                                           "Robot",
                                         "email":
                                           "contributors@kaneton.opaak.org",
                                         "login":
                                           "robot"
                                         } ])

  # store it.
  ktp.capability.Store(path,
                       capability)

  # remove the previous link.
  env.remove(link, env.OPTION_NONE)

  # create a link from the profile. XXX[this could probably be improved]
  env.link(link,
           "../../../../test/store/capability/" + name +                
             ktp.capability.Extension,
           env.OPTION_NONE)

  # display.
  env.display(env.HEADER_OK,
              "the robot's capability has been generated succesfully",
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
  usage()

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
