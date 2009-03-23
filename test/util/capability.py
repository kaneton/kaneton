#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /data/mycure/repositories/kaneton/test/util/capability.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [mon mar 23 12:23:46 2009]
#

#
# ---------- information ------------------------------------------------------
#
# this program generates capabilities for every student.
#

#
# ---------- packages ---------------------------------------------------------
#

import env
import ktc

import sys
import re

#
# ---------- globals ----------------------------------------------------------
#

g_school = None
g_year = None

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
              "usage: capability.py [school::year]",
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
# a capability for every student.
#
def                     generate(code,
                                 students):
  student = None
  id = None
  file = None

  # display.
  env.display(env.HEADER_OK,
              "generating the capabilities",
              env.OPTION_NONE)

  # for every student, generate a capability and store it.
  for student in students:
    # compute the id.
    id = student["name"].replace(" ", ".").lower()

    # compute the file name.
    file = env._TEST_STORE_CAPABILITY_DIR_ + "/" + id

    #
    # if a capability already exists.
    #
    if env.path(file + ".cap", env.OPTION_EXIST):
      # load it.
      capability = ktc.Load(file)

      # extract the object.
      object = ktc.Extract(capability)

      # if the information are the same, do not re-generate
      # a capability.
      if (object["id"] == id) and                                       \
         (object["school"] == g_school) and                             \
         (object["year"] == g_year) and                                 \
         (object["group"] == student["group"]) and                      \
         (object["name"] == student["name"]) and                        \
         (object["email"] == student["email"]):
        continue

    #
    # otherwise, generate a capability and store it.
    #

    # create the capability.
    capability = ktc.Capability(code,
                                id,
                                g_school,
                                g_year,
                                student["group"],
                                student["name"],
                                student["email"])

    # store it.
    ktc.Dump(file,
             capability)

    # display.
    env.display(env.HEADER_OK,
                "  " +
                id + " :: " +
                g_school + " :: " +
                g_year + " :: " +
                student["group"] + " :: " +
                student["name"] + " :: " +
                student["email"],
                env.OPTION_NONE)

#
# contributor()
#
# this function generates a capability for the contributor to
# test the research implementation as many times as they wish.
#
def                     contributor(code):
  id = None

  # display.
  env.display(env.HEADER_OK,
              "generating the contributor's capability",
              env.OPTION_NONE)

  # compute the id.
  id = "contributor"

  # compute the file name.
  file = env._TEST_STORE_CAPABILITY_DIR_ + "/" + id

  #
  # if a capability already exists.
  #
  if env.path(file + ".cap", env.OPTION_EXIST):
    # ignore this step and return.
    return

  #
  # otherwise, generate a capability and store it.
  #

  # create the capability.
  capability = ktc.Capability(code,
                              id,
                              "research",
                              42,
                              "kaneton",
                              "contributor",
                              "admin@opaak.org")

  # store it.
  ktc.Dump(file,
           capability)

  # display.
  env.display(env.HEADER_OK,
              "contributor's capability generated and stored",
              env.OPTION_NONE)

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
              "generating capabilities",
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # display a warning.
  warning()

  # retrieve the server's code.
  code = ktc.Read(env._TEST_STORE_CODE_DIR_ + "/server")

  # read the file and extract the students information.
  students = extract(path)

  # generate the capabilities.
  generate(code, students)

  # create a capability for the contributors.
  contributor(code)

  # display.
  env.display(env.HEADER_OK,
              "capabilities generated and stored",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
