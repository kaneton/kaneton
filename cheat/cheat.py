#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/cheat/cheat.py
#
# created       julien quintard   [thu may 24 01:40:40 2007]
# updated       julien quintard   [wed may  4 12:55:00 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this tool takes a school, year and stage and compares every student snapshot
# of the given school and year with the other snapshots of the current and
# past years.
#
# for more information, please refer to the development book.
#

#
# ---------- imports ----------------------------------------------------------
#

import sys
import re
import string
import random
import difflib

import env

#
# ---------- globals ----------------------------------------------------------
#

g_stages = [ "k0", "k1", "k2", "k3", "k4" ]

g_school = None
g_year = None
g_stage = None

g_base = None

g_directory = None

g_history = []

g_output = None
g_descriptor = None

g_targets = None

#
# ---------- constants --------------------------------------------------------
#

RANDOM_LENGTH = 32

THRESHOLD_BENIGN = 30
THRESHOLD_SUSPECT = 70
THRESHOLD_CRITIC = 100

LEVEL_BENIGN = "benign"
LEVEL_SUSPECT = "suspect"
LEVEL_CRITIC = "critic"

HTML_HEADER = """
<html>
  <head>
"""

HTML_FOOTER = """
  </head>
</html>
"""

HTML_STYLE = """
<style type="text/css">
  .comment
  {
    color: #cd2222;
    font-style: italic;
  }

  .preprocessor
  {
    color: blue;
  }

  .string
  {
    color: #009900;
  }

  .character
  {
    color: #009900;
  }

  .type
  {
    color: #FF6633;
  }

  .keyword
  {
    color: #58bbc1;
    font-weight: bold;
  }

  BODY
  {
    background-color: #eeeeee;
  }

  .title
  {
    text-align: right;
    font-style: italic;
    font-size: 15px;
  }

  .instance
  {
  }

  .student
  {
    font-size: 20px;
  }

  .bar
  {
    border-top: 1px solid #000000;
    margin-top: 2px;
    padding-top: 2px;
  }

  .people
  {
    margin-left: 5px;
    text-style: italic;
    font-size: 15px;
  }

  .categories
  {
    margin-left: 10px;
  }

  .category-critic
  {
    width: inherit;
    border: 1px solid #e51616;
  }

  .category-suspect
  {
    width: inherit;
    border: 1px solid #d7801a;
  }

  .category-benign
  {
    width: inherit;
    border: 1px solid #d0ca22;
  }

  .header-critic
  {
    background-color: #e51616;
    color: #cccccc;
  }

  .header-suspect
  {
    background-color: #d7801a;
    color: #cccccc;
  }

  .header-benign
  {
    background-color: #d0ca22;
    color: #cccccc;
  }

  .symbol A
  {
    font-weight: bold;
    float: left;
    text-decoration: none;
    color: inherit;
    margin-left: 3px;
    margin-bottom: 2px;
  }

  .matches
  {
    color: #eeeeee;
    font-size: 65%;
    font-style: italic;
    float: right;
    margin-right: 5px;
  }

  .traces
  {
    width: inherit;
    display: none;
    font-weight: normal;
    color: #000000;
    padding: 10px 10px 10px 10px;
    background-color: #eeeeee;
  }

  .description
  {
    width: 100%;
  }

  .trace
  {
    width: 100%;
    display: none;
    border: 1px dashed #000000;
  }

  .left
  {
    width: 50%;
  }

  .right
  {
    width: 50%;
    border-left: 1px dotted #000000;
  }

  .sources
  {
    margin: 10px 10px 10px 10px;
  }

  .me
  {
    text-align: left;
    width: 45%;
    font-weight: bold;
    font-size: 80%;
  }

  .tokens
  {
    text-align: center;
    font-style: italic;
    font-weight: normal;
    font-size: 70%;
    width: 10%;
  }

  .tokens A
  {
    text-decoration: none;
    color: inherit;
  }

  .you
  {
    text-align: right;
    width: 45%;
    font-weight: bold;
    text-decoration: none;
    color: inherit;
    font-size: 80%;
  }

  .clear
  {
    clear: both;
  }
</style>
"""

HTML_SCRIPT = """
<script type="text/javascript">
  function Show(id)
  {
    symbol = document.getElementById("symbol:" + id);
    traces = document.getElementById("traces:" + id);

    symbol.setAttribute("href", "javascript:Hide('" + id + "');");

    traces.style.display = "block";
  }

  function Hide(id)
  {
    symbol = document.getElementById("symbol:" + id);
    traces = document.getElementById("traces:" + id);

    symbol.setAttribute("href", "javascript:Show('" + id + "');");

    traces.style.display = "none";
  }

  function Open(id)
  {
    symbol = document.getElementById("symbol:" + id);
    trace = document.getElementById("trace:" + id);

    symbol.setAttribute("href", "javascript:Close('" + id + "');");

    trace.style.display = "table";
  }

  function Close(id)
  {
    symbol = document.getElementById("symbol:" + id);
    trace = document.getElementById("trace:" + id);

    symbol.setAttribute("href", "javascript:Open('" + id + "');");

    trace.style.display = "none";
  }
</script>
"""

HTML_INNER = """
  </head>
  <body>
"""

STATUS_INSERT="+"
STATUS_DELETE="-"
STATUS_EQUAL=" "

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
              "usage: cheat.py [school]::[year]::[stage]",
              env.OPTION_NONE)

#
# warning()
#
# this function displays the configuration.
#
def			warning():
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_OK, "configuration:", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  school:                 " + g_school,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  year:                   " + g_year,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  stage:                  " + g_stage,
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

#
# check()
#
# this function checks that the school, year and stage are valid.
#
def                     check():
  # check the school directory.
  if not (env.path(env._HISTORY_DIR_ + "/" + g_school,
                   env.OPTION_EXIST)):
    env.display(env.HEADER_ERROR,
                "the school '" + g_school + "' does not exist or is not a " +
                "directory",
                env.OPTION_NONE)
    sys.exit(42)

  # check the year directory.
  if not (env.path(env._HISTORY_DIR_ + "/" + g_school + "/" + g_year,
                   env.OPTION_EXIST)):
    env.display(env.HEADER_ERROR,
                "the year '" + g_year + "' does not exist or is not a " +
                "directory",
                env.OPTION_NONE)
    sys.exit(42)

  # check the stage.
  if not (g_stage in g_stages):
    env.display(env.HEADER_ERROR,
                "the stage '" + g_stage + "' is invalid, should be one of " +
                "the following: " + str(g_stages),
                env.OPTION_NONE)
    sys.exit(42)

#
# transform()
#
# this function takes a text and transform it so that it can be displayed in
# HTML.
#
def                     transform(contents):
  # strip the newline.
  contents = contents.strip("\n")

  # replace < and >.
  contents = contents.replace("<", "&lt;").replace(">", "&gt;")

  return contents

#
# prepare()
#
# this function prepares the additional tools as well as the file system.
#
def                     prepare():
  global g_history
  global g_directory
  global g_output
  global g_base
  years = None
  year = None
  stages = None
  stage = None
  base = None

  # a message.
  env.display(env.HEADER_OK,
              "preparing the verification process",
              env.OPTION_NONE)

  # create a temporary directory for holding unpacked snapshots.
  g_directory = env.temporary(env.OPTION_DIRECTORY)

  # set the output file path.
  g_output = env._HISTORY_DIR_ + "/" + g_school + "/" + g_year + "/" +  \
             g_stage + ".html"

  # build the CTC tool
  env.launch(env._CTC_DIR_ + "/Makefile", "", env.OPTION_QUIET)

  # look for a base snapshot.
  if env.path(env._HISTORY_DIR_ + "/" + g_school + "/" + g_year +       \
              "/snapshot.tar.bz2", env.OPTION_EXIST):
    g_base = { "snapshot": env._HISTORY_DIR_ + "/" + g_school + "/" + \
                 g_year + "/snapshot.tar.bz2",
               "path": g_directory + "/snapshot" }

    env.mkdir(g_base["path"], env.OPTION_NONE)
  else:
    g_base = None

  # retreive the list of other snapshots to compare. neighbours are the
  # students in the same year while alumni are old students.
  years = sorted(env.list(env._HISTORY_DIR_ + "/" + g_school,
                          env.OPTION_DIRECTORY))

  for year in years:
    # skip the younger works.
    if year > g_year:
      continue

    # for the work from the same year, simply compare the same stages.
    if year == g_year:
      # retrieve the students in this year.
      students = sorted(env.list(env._HISTORY_DIR_ + "/" +              \
                                   g_school + "/" + year,
                                 env.OPTION_DIRECTORY))

      # for every student.
      for student in students:
        # add the student to the list.
        g_history += [                                                  \
          {
            "name": year + "/" + student + "/" + g_stage,
            "year": year,
            "student": student,
            "stage": g_stage,
            "people": transform(env.pull(env._HISTORY_DIR_ + "/" +      \
                                         g_school + "/" + year + "/" +  \
                                         student + "/people",
                                         env.OPTION_NONE)),
            "snapshot": env._HISTORY_DIR_ + "/" + g_school + "/" + year + "/" +
                        student + "/sources/" + g_stage + ".tar.bz2",
            "sources": g_directory + "/" + year + "/" +                 \
                       student + "/" + g_stage,
            "fingerprint": g_directory + "/" + year + "/" + student + "/" +
                           g_stage + ".ctf",
            "base": base,
            "database": g_directory + "/" + year + "/" + student + "/tokens",
            "trace": g_directory + "/" + year + "/" + student + "/trace"
          } ]

    # for older work, take the most recent work from every older student.
# XXX this feature has been disabled because it generates too much code to process.
    if (year < g_year) and (0 == 1):
      # retrieve the students in this year.
      students = sorted(env.list(env._HISTORY_DIR_ + "/" + g_school + "/" + year,
                                 env.OPTION_DIRECTORY))

      # for every student.
      for student in students:
        # find the works for this student.
        stages = sorted(env.list(env._HISTORY_DIR_ + "/" + g_school + "/" +
                                 year + "/" + student + "/sources/",
                                 env.OPTION_FILE))

        # ignore students that skipped the project.
        if len(stages) == 0:
          continue

        # find the most recent work.
        match = re.match("^(.*)\.tar\.bz2$", stages[len(stages) - 1])

        if not match:
          continue

        # retrieve the stage name.
        stage = match.group(1)

        # add the last student's work to the list.
        g_history += [                                                  \
          {
            "name": year + "/" + student + "/" + stage,
            "year": year,
            "student": student,
            "stage": stage,
            "people": transform(env.pull(env._HISTORY_DIR_ + "/" +      \
                                         g_school + "/" + year + "/" +  \
                                         student + "/people",
                                         env.OPTION_NONE)),
            "snapshot": env._HISTORY_DIR_ + "/" + g_school + "/" + year + "/" +
                        student + "/sources/" + stage + ".tar.bz2",
            "sources": g_directory + "/" + year + "/" + student + "/" + stage,
            "fingerprint": g_directory + "/" + year + "/" + student + "/" +
                           stage + ".ctf",
            "base": base
          } ]

#
# extract()
#
# this function takes all the neighbours snapshots for the given stage and
# all the alumni snapshots for all stages and extract them.
#
def                     extract():
  student = None
  base = None

  # display a message.
  env.display(env.HEADER_OK,
              "extracting the students' snapshots",
              env.OPTION_NONE)

  # extract the history snapshots.
  for student in g_history:
    # message.
    env.display(env.HEADER_OK,
                "  " + student["name"],
                env.OPTION_NONE)

    env.mkdir(student["sources"], env.OPTION_NONE)

    if env.path(student["snapshot"], env.OPTION_EXIST):
      env.unpack(student["snapshot"],
                 student["sources"],
                 env.OPTION_QUIET)
    else:
      env.mkdir(student["sources"] + "/kaneton/", env.OPTION_NONE)

    # verify that the extracted snapshot has created a 'kaneton/' directory.
    if not (env.path(student["sources"] + "/kaneton/",
                     env.OPTION_EXIST)):
      env.display(env.HEADER_ERROR,
                  "the extracted student snapshot '" + student["name"] +
                  "' does not contain a 'kaneton/' directory",
                  env.OPTION_NONE)

    if len(env.list(student["sources"],
                    env.OPTION_FILE | env.OPTION_DIRECTORY)) != 1:
      env.display(env.HEADER_ERROR,
                  "the extracted student snapshot '" + student["name"] +
                  "' contains more than just the 'kaneton/' directory",
                  env.OPTION_NONE)

  # extract the bases.
  if g_base:
    env.unpack(g_base["snapshot"],
               g_base["path"],
                 env.OPTION_QUIET)

    # verify that the extracted snapshot has created a 'kaneton/' directory.
    if not (env.path(g_base["path"] + "/kaneton/",
                     env.OPTION_EXIST)):
      env.display(env.HEADER_ERROR,
                  "the extracted base snapshot '" + g_base["snapshot"] +
                  "' does not contain a 'kaneton/' directory",
                  env.OPTION_NONE)

    if len(env.list(g_base["path"],
                    env.OPTION_FILE | env.OPTION_DIRECTORY)) != 1:
      env.display(env.HEADER_ERROR,
                  "the extracted base snapshot '" + g_base["snapshot"] +
                  "' contains more than just the 'kaneton/' directory",
                  env.OPTION_NONE)

#
# generate()
#
# this function generates the extracted snapshots' fingerprint.
#
def                     generate():
  snapshot = None
  student = None
  file = None

  # display a message.
  env.display(env.HEADER_OK,
              "generating the snapshots' fingerprints",
              env.OPTION_NONE)

  # generate the token files.
  for student in g_history:
    # message.
    env.display(env.HEADER_OK,
                "  " + student["name"],
                env.OPTION_NONE)

    # create the token file
    env.launch(env._CTC_BUILD_TOOL_,
               "'" + student["sources"] + "/kaneton/'" + " " +
               "'" + student["fingerprint"] + "'",
               env.OPTION_QUIET)

  # display a message.
  env.display(env.HEADER_OK,
              "generating the snapshots' tokens database",
              env.OPTION_NONE)

  # generate the database file.
  for student in g_history:
    # if this student is part of the ones to test, continue.
    if (student["year"] == g_year) and                          \
       (student["stage"] == g_stage):
      fingerprints = []

      # message.
      env.display(env.HEADER_OK,
                  "  " + student["name"],
                  env.OPTION_NONE)

      # find the list of student snaphosts that will be tested against it.
      for snapshot in g_history:
        # skip the previous stages of the same user.
        if (student["year"] == snapshot["year"]) and            \
           (student["student"] == snapshot["student"]) and      \
           (student["stage"] != snapshot["stage"]):
          continue

        # add the fingerprint to the list.
        fingerprints += [ snapshot["fingerprint"] ]

      # finally, gather the information into a single database.
      env.launch(env._CTC_GATHER_TOOL_,
                 student["database"] + " " + " ".join(fingerprints),
                 env.OPTION_QUIET)

#
# compare()
#
# this function compares the snapshots for the given year and stage
# with other extract snapshots.
#
def                     compare():
  student = None
  target = None

  # display a message.
  env.display(env.HEADER_OK,
              "comparing the snapshots with each others",
              env.OPTION_NONE)

  # for every snapshot to test, generate the trace of comparisons against
  # other snapshots.
  if g_targets: 
    for target in g_targets:
      print target

      student = None

      # retrieve the student structure.
      for student in g_history:
        if target == student["student"]:
          break

      if not student:
        env.display(env.HEADER_ERROR,
                    "the student '" + target + "' does not seem to exist",
                    env.OPTION_NONE)
        sys.exit(42)

      print student

      if (student["year"] == g_year) and                     \
         (student["stage"] == g_stage):
        # message.
        env.display(env.HEADER_OK,
                    "  " + student["name"],
                    env.OPTION_NONE)

        # launch the program.
        env.launch(env._CTC_COMPARE_TOOL_,
                   student["trace"] + " " + student["database"] + " -r " +
                   student["fingerprint"],
                   env.OPTION_QUIET)
  else:
    for student in g_history:
      if (student["year"] == g_year) and                     \
         (student["stage"] == g_stage):
        # message.
        env.display(env.HEADER_OK,
                    "  " + student["name"],
                    env.OPTION_NONE)

        # launch the program.
        env.launch(env._CTC_COMPARE_TOOL_,
                   student["trace"] + " " + student["database"] + " -r " +
                   student["fingerprint"],
                   env.OPTION_QUIET)

#
# highlight()
#
# this function highlight the given C source file syntax.
#
def                     highlight(code, regexp, id):
  match = None
  offset = 0

  # look for all the given regexp-matching regions and apply
  # a <span> tag.
  while True:
    match = re.search(regexp, code[offset:], re.DOTALL | re.MULTILINE)

    if not match:
      break

    code = code[0:offset + match.start(1)] + "<span class='" + id +     \
                "'>" + match.group(1) + "</span>" +                     \
                code[offset + match.end(1):]

    offset = offset + match.start(1) + len("<span class='" + id +       \
             "'>") + len(match.group(1)) + len("</span>")

  return code

#
# region()
#
# this function returns the region of a given source file.
#
def                     region(file, start, end):
  handle = None
  line = None
  counter = None

  # open the file.
  if not env.path(file, env.OPTION_EXIST):
    return None

  try:
    handle = open(file, "r")
  except IOError:
    return None

  content = ""
  counter = 1

  for line in handle.readlines():
    # only keep the lines lying between start and end.
    if (counter >= start) and (counter <= end):
      content += line

    counter = counter + 1

  handle.close()

  # convert.
  content = transform(content)

  # highlight C comments.
  content = highlight(content, r"(\/\*.*?\*\/)", "comment")

  # highlight C++ comments.
  content = highlight(content, r"(\/\/.*?)$", "comment")

  # hightlight pre-processor directives.
  content = highlight(content, r"^(#[A-Za-z0-9_]+).*?$", "preprocessor")

  # highlight strings.
  content = highlight(content, r"(\"(?:\\\"|.)*?\")", "string")

  # highlight characters.
  content = highlight(content, r"(\'.\')", "character")

  # highlight types.
  content = highlight(content,
                      r"[^A-Za-z0-9](bool|char|double|float|int|long|"
                       "short|signed|unsigned|void)[^A-Za-z0-9]",
                      "type")

  # highlight keywords.
  content = highlight(content,
                      r"[^A-Za-z0-9](break|case|continue|default|do|else|"
                       "for|goto|if|return|switch|while|asm|const|enum|"
                       "struct|extern|inline|sizeof|static|typedef|union|"
                       "volatile)[^A-Za-z0-9]",
                      "keyword")

  return content

#
# initialise()
#
# this function opens the output HTML file.
#
def                     initialise():
  global g_descriptor

  g_descriptor = open(g_output, "w")

#
# clean()
#
# this function closes the HTML file.
#
def                     clean():
  global g_descriptor

  g_descriptor.close()

  g_descriptor = None

#
# write()
#
# this function writes in the HTML file.
#
def                     write(contents):
  g_descriptor.write(contents)

#
# identifier()
#
# this function returns an identifier from a path.
#
def                     identifier(path):
  match = None

  # each path is of the following form:
  #   ${temporary-dir}/${year}/${student}/${stage}/kaneton/${file}
  #
  # the following extract the year/student/stage and file information.
  match = re.search("^" + g_directory + "/(.+?/.+?/.+?)/kaneton/(.+)$", path)

  return { "name": match.group(1),
           "file": match.group(2) }

#
# label()
#
# this function generates a random label.
#
def                     label():
  alphabet = None
  label = None
  i = None

  # generate a random string.
  alphabet = string.ascii_lowercase + string.digits

  for i in range(RANDOM_LENGTH):
    if not label:
      label = alphabet[random.randint(0, len(alphabet) - 1)]
    else:
      label = label + alphabet[random.randint(0, len(alphabet) - 1)]

  return label

#
# build()
#
# this function takes a list of records a organises them in a
# meaningful trace.
#
def                     build(name, data):
  trace = { LEVEL_CRITIC: [], LEVEL_SUSPECT: [], LEVEL_BENIGN: [] }
  record = None

  for record in data:
    level = None
    match = None

    # look for a record
    match = re.match("^(([0-9]+)"
                     " +"
                     "([^:]+):([0-9]+)-([0-9]+)"
                     " +"
                     "([^:]+):([0-9]+)-([0-9]+))$",
                     record)

    if not match:
      continue

    # retrieve the arguments.
    tokens = int(match.group(2))

    # determine the level of this record.
    if tokens >= THRESHOLD_CRITIC:
      level = LEVEL_CRITIC
    elif tokens >= THRESHOLD_SUSPECT:
      level = LEVEL_SUSPECT
    elif tokens >= THRESHOLD_BENIGN:
      level = LEVEL_BENIGN
    else:
      continue

    # add the record to the trace, taking care that 'student' represents the
    # student side.
    if name == identifier(match.group(3))["name"]:
      trace[level] +=                                                   \
        [ { "label": label(),
            "tokens": tokens,
            "student": { "path": match.group(3),
                         "from": int(match.group(4)),
                         "to": int(match.group(5)),
                         "name": identifier(match.group(3))["name"],
                         "file": identifier(match.group(3))["file"],
                         "code": region(match.group(3),
                                        int(match.group(4)),
                                        int(match.group(5))) },
            "neighbour": { "path": match.group(6),
                           "from": int(match.group(7)),
                           "to": int(match.group(8)),
                           "name": identifier(match.group(6))["name"],
                           "file": identifier(match.group(6))["file"],
                           "code": region(match.group(6),
                                          int(match.group(7)),
                                          int(match.group(8))) } } ]
    else:
      trace[level] +=                                                   \
        [ { "label": label(),
            "tokens": tokens,
            "neighbour": { "path": match.group(3),
                           "from": int(match.group(4)),
                           "to": int(match.group(5)),
                           "name": identifier(match.group(3))["name"],
                           "file": identifier(match.group(3))["file"],
                           "code": region(match.group(3),
                                          int(match.group(4)),
                                          int(match.group(5))) },
            "student": { "path": match.group(6),
                         "from": int(match.group(7)),
                         "to": int(match.group(8)),
                         "name": identifier(match.group(6))["name"],
                         "file": identifier(match.group(6))["file"],
                         "code": region(match.group(6),
                                        int(match.group(7)),
                                        int(match.group(8))) } } ]

  return trace

#
# output()
#
# this function takes the traces and generates a human-readable user-friendly
# HTML page.
#
def                     output():
  student = None

  # display a message.
  env.display(env.HEADER_OK,
              "generating the output file '" + g_output + "'",
              env.OPTION_NONE)

  # initialise the output file.
  initialise()

  # write the basic info.
  write(HTML_HEADER)
  write("<title>[" + g_school + " :: " + g_year + " :: " +
        g_stage + "]</title>")
  write(HTML_STYLE)
  write(HTML_SCRIPT)
  write(HTML_INNER)

  # for every student to test, retrieve the trace and include it in the
  # output HTML page.
  for student in g_history:
    data = None
    trace = None

    # only treat the student from the given year and stage.
    if not ((student["year"] == g_year) and                     \
            (student["stage"] == g_stage)):
      continue

    # load the trace.
    data = env.pull(student["trace"], env.OPTION_NONE).strip("\n").split("\n")

    # build a meaningful trace from the chaotic records
    trace = build(student["name"], data)

    # initialise the output for this student.
    write("""
<div class="instance">
  <span class="student">""" + student["student"] + """</span>
  <span class="people">""" + student["people"] + """</span>
  <div class="categories">
""")

    # go through every level of the trace.
    for level in [ LEVEL_CRITIC, LEVEL_SUSPECT, LEVEL_BENIGN ]:
      L = None

      # skip empty categories.
      if len(trace[level]) == 0:
        continue

      # generate a label for the category.
      L = label()

      # output the category header.
      write("""
<div class="category-""" + level + """">
  <div class="header-""" + level + """">
    <div class="symbol"><a href="javascript:Show('""" + L +
"""');" id="symbol:""" + L + """">+</a></div>
    <div class="matches">""" + str(len(trace[level])) + """ matches</div>
    <div class="clear"></div>
  </div>
  <div class="traces" id="traces:""" + L + """">
""")

      # for every record in this level.
      for record in trace[level]:
        bar = None

        # is this the first record in this level?
        if record != trace[level][0]:
          bar = " bar"
        else:
          bar = ""

        # write the content.
        write("""
<table class="description""" + bar +
"""" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td class="me">""" +
record["student"]["name"] + """ :: """ + record["student"]["file"] + """</td>
    <td class="tokens"><a href="javascript:Open('""" + record["label"] +
"""');" id="symbol:""" + record["label"] + """">""" + str(record["tokens"]) +
""" tokens</a></td>
    <td class="you">""" + record["neighbour"]["file"] + """ :: """ +
record["neighbour"]["name"] + """</td>
  </tr>
</table>
<table class="trace" id="trace:""" + record["label"] + """">
  <tr>
    <td class="left">
      <pre class="sources">""" + record["student"]["code"] + """</pre>
    </td>
    <td class="right">
      <pre class="sources">""" + record["neighbour"]["code"] + """</pre>
    </td>
  </tr>
</table>
""")

      # close the category.
      write("""
      </div>
    </div>
""")

    # finish the generation for this student.
    write("""
  </div>
</div>
<br/>
""")

  # finish the HTML generation.
  write(HTML_FOOTER)

  clean()

#
# reduce()
#
# this function reduces the given student file according to the
# reference.
#
def             reduce(student, reference):
  differences = None
  contents = None
  differ = None
  status = None
  line = None
  r = None
  s = None

  if not ((env.path(reference, env.OPTION_EXIST)) and                   \
          (env.path(reference, env.OPTION_FILE))):
    return

  # load the reference and student files.
  r = env.pull(reference, env.OPTION_NONE).split("\n")
  s = env.pull(student, env.OPTION_NONE).split("\n")

  # build an differ object.
  differ = difflib.Differ()

  # look for the differences.
  differences = differ.compare(r, s)

  # only keep the added lines.
  status = STATUS_EQUAL

  contents=""

  # for each line.
  for line in differences:
    if line[len(line) - 1] == "\n":
      line = line[:len(line) - 1]

    # we insert a CTC-GATE token so that the CTC tool does not match code
    # regions including such a token.
    # this is done because code has been removed. therefore, a match has
    # no sense here.
    if (line[0] == STATUS_INSERT) and (line[0] != status):
      contents += "_____ctc_gate_____\n";

    status = line[0]

    # add the line to the final file since we only keep added stuff.
    if line[0] == STATUS_INSERT:
      contents += line[2:] + "\n"

  # save the contents in the student file.
  env.push(student, contents, env.OPTION_NONE)

#
# walk()
#
# this function takes a directory and two static directories: the
# student sources and the base sources. the goal of this function
# is to remove parts that were given in the base sources.
#
def             walk(snapshot, base, directory):
  student = None
  reference = None
  directories = None
  files = None
  entry = None

  # adjust the directory:
  if not directory:
    directory = ""

  # retrieve the list of directories and files.
  files = env.list(snapshot + "/" + directory,
                   env.OPTION_FILE | env.OPTION_HIDDEN)

  # try to reduce every file
  for entry in files:
    reduce(snapshot + "/" + directory + "/" + entry,
           base + "/" + directory + "/" + entry)

  # keep walking through the hierarchy
  directories = env.list(snapshot + "/" + directory,
                         env.OPTION_DIRECTORY | env.OPTION_HIDDEN)

  for entry in directories:
    walk(snapshot, base, directory + "/" + entry)

#
# filter()
#
# this function removes the files and directories that should not be
# taken into consideration by the cheat verification system.
#
def             filter(student):
  entities = None
  entity = None

  # remove the env._CHEAT_FILTER_ stuff
  entities = re.findall("([^\t ]+)", env._CHEAT_FILTER_)

  for entity in entities:
    env.remove(student["sources"] + "/kaneton/" + entity, env.OPTION_NONE)

#
# shear()
#
# this function walks through the extracted snapshots and tries to
# remove the parts that have been given to the students as a basis.
#
def             shear():
  # display a message.
  env.display(env.HEADER_OK,
              "shearing the students' snaphosts",
              env.OPTION_NONE)

  # for each student.
  for student in g_history:
    # message.
    env.display(env.HEADER_OK,
                "  " + student["name"],
                env.OPTION_NONE)

    # remove the extra useless stuff.
    filter(student)

    # continue only if the student is part of the given year
    # and if a base snapshot is provided for that year.
    if (student["year"] == g_year) and g_base:
      # remove the common parts.
      walk(student["sources"] + "/kaneton",
           g_base["path"] + "/kaneton",
           None)

#
# clear()
#
# this function clears the files and directories created by the
# verification process.
#
def                     clear():
  # remove the temporary directory which contains all the extracted
  # snapshots etc.
  env.remove(g_directory, env.OPTION_NONE)

#
# main()
#
# this function performs the main stuff.
#
def			main():
  global g_school
  global g_year
  global g_stage
  args = None

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  # retrive the actual arguments.
  args = sys.argv[1].split("::")

  # check the number of arguments.
  if len(args) != 3:
    usage()
    sys.exit(42)

  # set the global variables.
  g_school = args[0]
  g_year = args[1]
  g_stage = args[2]

  # check that the school and year are valid.
  check()

  # warn the user about what is about to happen.
  warning()

  # prepare the process.
  prepare()

  # extract all the student snapshots in a temporary directory so that
  # the CTC tool can access the sources.
  extract()

  # shear the snapshots according to the year's base snapshot.
  shear()

  # generate the token files and the database.
  generate()

  # compare the given snapshots with the other ones, generating cheat
  # traces.
  compare()

  # generate the HTML output.
  output()

  # clear the temporary files and directories.
  clear()

  # final message.
  env.display(env.HEADER_OK,
              "cheating verification processed successfully",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
