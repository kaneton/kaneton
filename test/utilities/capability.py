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
# updated       julien quintard   [sat feb  5 00:07:26 2011]
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
# extract()
#
# this function takes the path to a history directory and
# extract the names, emails and id of every student.
#
# finally, the function returns the information in a data
# structure.
#
def                     extract(path):
  container = {}
  students = None
  members = None
  member = None
  people = None

  # display.
  env.display(env.HEADER_OK,
              "extracting the students from the history '" + path + "'",
              env.OPTION_NONE)

  # retrieve the list of students.
  students = env.list(env._HISTORY_DIR_ + "/" + path,
                      env.OPTION_DIRECTORY)

  # for each student, extract the information from the
  # 'people' file.
  for student in students:
    # read the 'people' file.
    people = env.pull(env._HISTORY_DIR_ + "/" + path + "/" +
                      student + "/people", env.OPTION_NONE)

    # extract every line
    members = people.strip("\n").split("\n")

    # initialize the people list.
    people = []

    # for every member, extract the first email email.
    for member in members:
      # apply a regexp to locate the elements.
      match = re.match("^(.+) <(.+)>$", member)

      if not match:
        env.display(env.HEADER_ERROR,
                    "  unable to extract the name and email from the "
                    "student '" + student + "'",
                    env.OPTION_NONE)

      # register the member.
      people += [ { "name": match.group(1),
                    "email": match.group(2) } ]

    # add the student.
    container[student] = people

  # display.
  env.display(env.HEADER_OK,
              "students information retrieved",
              env.OPTION_NONE)

  return container

#
# personal()
#
# this function takes an existing capability and generates
# a personal capability for everyone of the members.
#
def                     personal(code,
                                 capability):
  members = None
  member = None
  name = None
  file = None
  community = None

  # display.
  env.display(env.HEADER_OK,
              "generating the students' personal capabilities",
              env.OPTION_NONE)

  # retrieve the members.
  members = capability["members"]

  # make the group capability identifier the community of the personal ones.
  community = capability["identifier"]

  # for every member.
  for member in members:
    # generate a name.
    name = member["name"].lower().replace(" ", ".")

    # compute the file name.
    file = env._TEST_STORE_CAPABILITY_DIR_ + "/" +                      \
        name + ktp.capability.Extension

    # create a specific capability for the member.
    capability = ktp.capability.Create(code,
                                       name,
                                       ktp.capability.TypeStudent,
                                       community,
                                       [ { "name": member["name"],
                                           "email": member["email"] } ])

    # store it.
    ktp.capability.Store(file,
                         capability)

    # display.
    env.display(env.HEADER_OK,
                "  " + name,
                env.OPTION_NONE)

  # display.
  env.display(env.HEADER_OK,
              "students' personal capabilities generated",
              env.OPTION_NONE)

#
# generate()
#
# this function takes a data structure and generate
# a capability for every student.
#
def                     generate(code,
                                 students):
  student = None
  name = None
  file = None

  # display.
  env.display(env.HEADER_OK,
              "generating the students' capabilities:",
              env.OPTION_NONE)

  # for every student, generate a capability and store it.
  for student in students:
    # compute the name.
    name = g_path.replace("/", "::") + "::" + student

    # compute the file name.
    file = env._TEST_STORE_CAPABILITY_DIR_ + "/" +                      \
        name + ktp.capability.Extension

    # create the capability.
    capability = ktp.capability.Create(code,
                                       name,
                                       ktp.capability.TypeGroup,
                                       "students",
                                       students[student])

    # store it.
    ktp.capability.Store(file,
                         capability)

    # display.
    env.display(env.HEADER_OK,
                "  " + name,
                env.OPTION_NONE)

    # generate the personal capabilities.
    personal(code, capability)

#
# student()
#
# this function generates a capability for a specific user.
#
def                     student():
  name = None
  email = None
  file = None
  people = None
  persons = []
  member = None
  members = None

  # display.
  env.display(env.HEADER_OK,
              "generating the student's group capability:",
              env.OPTION_NONE)

  # compute the name.
  name = g_path.strip("/").replace("/", "::")

  # read the 'people' file.
  people = env.pull(env._HISTORY_DIR_ + "/" + g_path + "/people",
                    env.OPTION_NONE)

  # extract every line
  members = people.strip("\n").split("\n")

  # for every member, extract name and email.
  for member in members:
    # apply a regexp to locate the elements.
    match = re.match("^(.+) <(.+)>$", member)

    if not match:
      env.display(env.HEADER_ERROR,
                  "  unable to extract the name and email from the student '" +
                  g_path + "'",
                  env.OPTION_NONE)
    else:
      # register the member.
      persons += [ { "name": match.group(1),
                     "email": match.group(2) } ]

  # compute the file name.
  file = env._TEST_STORE_CAPABILITY_DIR_ + "/" +                        \
      name + ktp.capability.Extension

  # retrieve the server's code.
  code = ktp.code.Load(env._TEST_STORE_CODE_DIR_ + "/server" +          \
                         ktp.code.Extension)

  # create the capability.
  capability = ktp.capability.Create(code,
                                     name,
                                     ktp.capability.TypeGroup,
                                     "students",
                                     persons)

  # store it.
  ktp.capability.Store(file,
                       capability)

  # display.
  env.display(env.HEADER_OK,
              "  " + name,
              env.OPTION_NONE)

  # display.
  env.display(env.HEADER_OK,
              "student's group capability generated and stored",
              env.OPTION_NONE)

  # generate the personal capabilities.
  personal(code, capability)

#
# school()
#
# this function generates a capability for a set of students identified
# by their school and year.
#
def                     school():
  # display.
  env.display(env.HEADER_OK,
              "generating students' capabilities",
              env.OPTION_NONE)

  # retrieve the server's code.
  code = ktp.code.Load(env._TEST_STORE_CODE_DIR_ + "/server" +          \
                         ktp.code.Extension)

  # read the file and extract the students information.
  students = extract(g_path)

  # generate the capabilities.
  generate(code, students)

  # display.
  env.display(env.HEADER_OK,
              "students' capabilities generated and stored",
              env.OPTION_NONE)

#
# contributor()
#
# this function generates a capability for the contributor to
# test the research implementation as many times as they wish.
#
def                     contributor():
  name = None

  # display.
  env.display(env.HEADER_OK,
              "generating the contributor's capability",
              env.OPTION_NONE)

  # retrieve the server's code.
  code = ktp.code.Load(env._TEST_STORE_CODE_DIR_ + "/server" +          \
                         ktp.code.Extension)

  # compute the name.
  name = "contributor"

  # compute the file name.
  file = env._TEST_STORE_CAPABILITY_DIR_ + "/" + name +                 \
      ktp.capability.Extension

  # create the capability.
  capability = ktp.capability.Create(code,
                                     name,
                                     ktp.capability.TypeStudent,
                                     "contributors",
                                     [ { "name":
                                           "contributors",
                                         "email":
                                           "contributors@kaneton.opaak.org"
                                         } ])

  # store it.
  ktp.capability.Store(file,
                       capability)

  # display.
  env.display(env.HEADER_OK,
              "contributor's capability generated and stored",
              env.OPTION_NONE)

#
# robot()
#
# this function generates a capability for the robot.
#
def                     robot():
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
  file = env._TEST_STORE_CAPABILITY_DIR_ + "/" + name +                 \
      ktp.capability.Extension

  # create the capability.
  capability = ktp.capability.Create(code,
                                     name,
                                     ktp.capability.TypeStudent,
                                     "contributors",
                                     [ { "name":
                                           "contributors",
                                         "email":
                                           "contributors@kaneton.opaak.org"
                                         } ])

  # store it.
  ktp.capability.Store(file,
                       capability)

  # display.
  env.display(env.HEADER_OK,
              "robot's capability generated and stored",
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
