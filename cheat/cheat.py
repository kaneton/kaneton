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
# updated       julien quintard   [wed sep  3 19:20:49 2008]
#

#
# ---------- information ------------------------------------------------------
#
# XXX
#

#
# ---------- imports ----------------------------------------------------------
#

import sys
import re
import string
import random

import env

#
# ---------- globals ----------------------------------------------------------
#

g_stages = [ "k0", "k1", "k2", "k3", "k4" ]

g_school = None
g_year = None
g_stage = None

g_directory = None
g_file = None

g_history = []

g_output = None
g_descriptor = None

# XXX
import yaml

def STORE():
  stream = {}
  descriptor = None

  stream["school"] = g_school
  stream["year"] = g_year
  stream["stage"] = g_stage
  stream["directory"] = g_directory
  stream["file"] = g_file
  stream["history"] = g_history
  stream["output"] = g_output

  descriptor = file(".dump", 'w')

  yaml.dump(stream, descriptor)

def LOAD():
  global g_school
  global g_year
  global g_stage
  global g_directory
  global g_file
  global g_history
  global g_output

  stream = yaml.load(env.pull(".dump", env.OPTION_NONE))

  g_school = stream["school"]
  g_year = stream["year"]
  g_stage = stream["stage"]
  g_directory = stream["directory"]
  g_file = stream["file"]
  g_history = stream["history"]
  g_output = stream["output"]

# XXX

#
# ---------- constants --------------------------------------------------------
#

RANDOM_LENGTH = 32

THRESHOLD_BENIGN = 20
THRESHOLD_SUSPECT = 50
THRESHOLD_CRITICAL = 100

LEVEL_BENIGN = "yellow"
LEVEL_SUSPECT = "orange"
LEVEL_CRITICAL = "red"

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

  .categories
  {
    margin-left: 10px;
  }

  .category-red
  {
    width: inherit;
    border: 1px solid #e51616;
  }

  .category-orange
  {
    width: inherit;
    border: 1px solid #d7801a;
  }

  .category-yellow
  {
    width: inherit;
    border: 1px solid #d0ca22;
  }

  .header-red
  {
    background-color: #e51616;
    color: #cccccc;
  }

  .header-orange
  {
    background-color: #d7801a;
    color: #cccccc;
  }

  .header-yellow
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

#
# ---------- functions --------------------------------------------------------
#

#
# usage()
#
# this function displays the usage.
#
def			usage():
  env.display(env.HEADER_NONE, "usage: cheat.py [school].[year].[stage]",
              env.OPTION_NONE)

#
# warning()
#
# this function asks the user if everything is right.
#
def			warning():
  env.display(env.HEADER_OK, "your current configuration:", env.OPTION_NONE)

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
  env.display(env.HEADER_INTERACTIVE,
              "to cancel press CTRL^C, otherwise press enter", env.OPTION_NONE)

  env.input(env.OPTION_NONE)

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
# prepare()
#
# this function prepares the additional tools as well as the file system.
#
def                     prepare():
  global g_history
  global g_directory
  global g_file
  global g_output
  years = None
  year = None
  stages = None
  stage = None
  base = None

  # a message.
  env.display(env.HEADER_OK,
              "preparing the verification process",
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # create a temporary directory for holding unpacked snapshots.
  g_directory = env.temporary(env.OPTION_DIRECTORY)
  g_file = env.temporary(env.OPTION_FILE)

  # set the output file path.
  g_output = env._HISTORY_DIR_ + "/" + g_school + "/" + g_year + "/" +  \
             g_stage + ".html"

  # build the CTC tool
  env.launch(env._CTC_DIR_ + "/Makefile", "", env.OPTION_QUIET)

  # retreive the list of other snapshots to compare. neighbours are the
  # students in the same year while alumni are old students.
  years = env.list(env._HISTORY_DIR_ + "/" + g_school, env.OPTION_DIRECTORY)

  for year in years:
    students = env.list(env._HISTORY_DIR_ + "/" + g_school + "/" + year,
                        env.OPTION_DIRECTORY)

    if year > g_year:
      continue

    if env.path(env._HISTORY_DIR_ + "/" + g_school + "/" + year +
                "/snapshot.tar.bz2", env.OPTION_EXIST):
      base = env._HISTORY_DIR_ + "/" + g_school + "/" + year +          \
             "/snapshot.tar.bz2"
    else:
      base = None

    for student in students:
      stages = env.list(env._HISTORY_DIR_ + "/" + g_school + "/" + year +
                        "/" + student,
                        env.OPTION_DIRECTORY)

      for stage in g_stages:
        if (year == g_year) and (stage > g_stage):
          continue

        g_history += [                                                  \
          {
            "name": year + "/" + student + "/" + stage,
            "year": year,
            "student": student,
            "stage": stage,
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
  element = None

  # extract the history snapshots.
  for element in g_history:
    env.mkdir(element["sources"], env.OPTION_NONE)

    if env.path(element["snapshot"], env.OPTION_EXIST):
      pass
      env.unpack(element["snapshot"],
                 element["sources"],
                 env.OPTION_NONE)
    else:
      env.mkdir(element["sources"] + "/kaneton/", env.OPTION_NONE)

    # verify that the extracted snapshot has created a 'kaneton/' directory.
    if not (env.path(element["sources"] + "/kaneton/",
                     env.OPTION_EXIST)):
      env.display(env.HEADER_ERROR,
                  "the extracted snapshot '" + element + "' does not "
                  "contain any 'kaneton/' directory",
                  env.OPTION_NONE)

    if len(env.list(element["sources"],
                    env.OPTION_FILE | env.OPTION_DIRECTORY)) != 1:
      env.display(env.HEADER_ERROR,
                  "the extracted snapshot '" + element + "' contains more "
                  "than just the 'kaneton/' directory",
                  env.OPTION_NONE)

#
# generate()
#
# this function generates the extracted snapshots' fingerprint.
#
def                     generate():
  fingerprints = []
  element = None

  # generate the token files.
  for element in g_history:
    env.launch("/home/mycure/CTC/ctc/buildctf",
               "'" + element["sources"] + "/kaneton/'" + " " +
               "'" + element["fingerprint"] + "'",
               env.OPTION_QUIET)

  for element in g_history:
    fingerprints += [ element["fingerprint"] ]

  # gather the information in a single database.
  env.launch("/home/mycure/CTC/ctc/enhashctf",
             g_file + " " + " ".join(fingerprints),
             env.OPTION_QUIET)

#
# compare()
#
# this function compares the snapshots for the given year and stage
# with other extract snapshots.
#
def                     compare():
  element = None

  # for every snapshot to test, generate the trace of comparisons against
  # other snapshots.
  for element in g_history:
    if (element["year"] == g_year) and                       \
       (element["stage"] == g_stage):
      # create a temporary file.
      element["trace"] = env.temporary(env.OPTION_FILE)

      # launch the program.
      env.launch("/home/mycure/CTC/ctc/ctcompare",
                 element["trace"] + " " + g_file + " -r " +
                 element["fingerprint"],
                 env.OPTION_NONE)

#
# highlight()
#
# this function highlight the given C source file syntax.
#
def                     highlight(code, regexp, id):
  match = None
  offset = 0

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

  #
  # load the file.
  #

  if not env.path(file, env.OPTION_EXIST):
    return None

  try:
    handle = open(file, "r")
  except IOError:
    return None

  content = ""
  counter = 1

  for line in handle.readlines():
    if (counter >= start) and (counter <= end):
      content += line

    counter = counter + 1

  handle.close()

  #
  # convert the file in HTML.
  #

  # convert < and > characters in HTML.
  content = content.replace("<", "&lt;")
  content = content.replace(">", "&gt;")

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
                      r"[^A-Za-z0-9](bool|char|double|float|int|long|short|signed|unsigned|void)[^A-Za-z0-9]",
                      "type")

  # highlight keywords.
  content = highlight(content,
                      r"[^A-Za-z0-9](break|case|continue|default|do|else|for|goto|if|return|switch|while|asm|const|enum|struct|extern|inline|sizeof|static|typedef|union|volatile)[^A-Za-z0-9]",
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

  match = re.search("^" + g_directory + "/(.+?/.+?/.+?)/kaneton/(.+)$", path)

  return { "name": match.group(1),
           "file": match.group(2) }

#
# rand()
#
# this function generates a random string.
#
def                     rand():
  alphabet = None
  id = None

  alphabet = string.letters + string.digits

  for i in range(RANDOM_LENGTH):
    if not id:
      id = alphabet[random.randint(0, len(alphabet) - 1)]
    else:
      id = id + alphabet[random.randint(0, len(alphabet) - 1)]

  return id

#
# output()
#
# this function takes the traces and generates a human-readable user-friendly
# HTML page.
#
def                     output():
  contents = None
  offset = 0
  lines = None
  line = None
  level = None

  # initialise the output file.
  initialise()

  # write the basic info.
  write(HTML_HEADER)
  write("<title>[" + g_school + " :: " + g_year + " :: " + g_stage + "]</title>")
  write(HTML_STYLE)
  write(HTML_SCRIPT)
  write(HTML_INNER)

  write("""<div class="title">[""" + g_school + " :: " + g_year + " :: " + g_stage + """]</div>""")

  # for every snapshot to test, retrieve the trace and include them in the
  # output HTML page.
  for element in g_history:
    if (element["year"] == g_year) and                       \
       (element["stage"] == g_stage):
      student = None

      # generate a student random identifier.
      student = rand()

      # load the trace.
      contents = env.pull(element["trace"], env.OPTION_NONE)

      # cut the contents into lines.
      lines = contents.strip("\n").split("\n")

      # initialise the output for this student.
      write("""
<div class="instance">
  <span class="student">""" + element["student"] + """</span>
  <div class="categories">
""")

      # for every record in the trace.
      for line in lines:
        source = None
        destination = None
        record = None
        tokens = None
        match = None
        id = None
        bar = None
        trace = None
        category = None

        # look for a record
        match = re.match("^(([0-9]+) +([^:]+):([0-9]+)-([0-9]+) +([^:]+):([0-9]+)-([0-9]+))$", line)
        if not match:
          continue

        # if this is not the first record, put a bar on top of this record.
        if line != lines[0]:
          bar = " bar"
        else:
          bar = ""

        # retrieve the arguments.
        record = match.group(1)

        tokens = int(match.group(2))

        # generate a trace identifier.
        trace = rand()

        # according to the matching token level, produce the proper category.
        if tokens < THRESHOLD_BENIGN:
          break

        # if this is the first category.
        if not level:
          if tokens >= THRESHOLD_CRITICAL:
            level = LEVEL_CRITICAL
          elif tokens >= THRESHOLD_SUSPECT:
            level = LEVEL_SUSPECT
          elif tokens >= THRESHOLD_BENIGN:
            level = LEVEL_BENIGN

          write("""
<div class="category""" + level + """">
  <div class="header""" + level + """">
    <div class="symbol"><a href="javascript:Show('""" + student + """');" id="symbol:""" + student + """">+</a></div>
    <div class="matches">""" + str(len(lines)) + """ matches</div>
    <div class="clear"></div>
  </div>
  <div class="traces" id="traces:""" + student + """">
""")
        # if this is a new category, close the old one and open the new one.
        else:
          if tokens >= THRESHOLD_CRITICAL:
            category = LEVEL_CRITICAL
          elif tokens >= THRESHOLD_SUSPECT:
            category = LEVEL_SUSPECT
          elif tokens >= THRESHOLD_BENIGN:
            category = LEVEL_BENIGN

          if level != category:
            write("""
      </div>
    </div>
""")

            level = category

            write("""
<div class="category""" + level + """">
  <div class="header""" + level + """">
    <div class="symbol"><a href="javascript:Show('""" + student + """');" id="symbol:""" + student + """">+</a></div>
    <div class="matches">""" + str(len(lines)) + """ matches</div>
    <div class="clear"></div>
  </div>
  <div class="traces" id="traces:""" + student + """">
""")

# XXX[len(lines) is wrong]

        # keep retrieving the arguments.
        source = { "path": match.group(3),
                   "from": int(match.group(4)),
                   "to": int(match.group(5)),
                   "name": identifier(match.group(3))["name"] + " :: " + identifier(match.group(3))["file"],
                   "code": region(match.group(3), int(match.group(4)), int(match.group(5))) }

        destination = { "path": match.group(6),
                        "from": int(match.group(7)),
                        "to": int(match.group(8)),
                        "name": identifier(match.group(6))["name"] + " :: " + identifier(match.group(6))["file"],
                        "code": region(match.group(6), int(match.group(7)), int(match.group(8))) }

        write("""
<table class="description """ + bar + """" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td class="me">""" + source["name"] + """</td>
    <td class="tokens"><a href="javascript:Open('""" + trace + """');" id="symbol:""" + trace + """">""" + str(tokens) + """ tokens</a></td>
    <td class="you">""" + destination["name"] + """</td>
  </tr>
</table>
<table class="trace" id="trace:""" + trace + """">
  <tr>
    <td class="left">
      <pre class="sources">""" + source["code"] + """</pre>
    </td>
    <td class="right">
      <pre class="sources">""" + destination["code"] + """</pre>
    </td>
  </tr>
</table>
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
  args = sys.argv[1].split(".")

  # set the global variables.
  g_school = args[0]
  g_year = args[1]
  g_stage = args[2]

  # check that the school and year are valid.
#  check()

  # warn the user about what is about to happen.
#  warning()

  # prepare the process.
#  prepare()

  # extract all the student snapshots in a temporary directory so that
  # the CTC tool can access the sources.
#  extract()

  # generate the token files and the database.
#  generate()

  # compare the given snapshots with the other ones, generating cheat
  # traces.
#  compare()

# XXX
#  STORE()
  LOAD()

  # generate the HTML output.
  output()

  # a final message.
  env.display(env.HEADER_OK,
              "cheating verification processed successfully",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
