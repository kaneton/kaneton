##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/forest.py
##
## created       julien quintard   [sun jul 30 15:35:02 2006]
## updated       julien quintard   [sat sep 16 13:17:51 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file contains the main routines of the forest program.
#
# notice that the 'options()' function calls the 'options()' function
# of the parsers module and checkers module. this action is performed
# to allow the parser and the checker to use additional options.
#
# the 'f' variable represents the main forest variable containing everything
# necessary for the program.
#

#
# ----- informations ----------------------------------------------------------
#

VERSION = "1.0"
AUTHOR = "mycure {julien.quintard@gmail.com}"
CONTRIBUTORS = ""

#
# ----- imports ---------------------------------------------------------------
#

from optparse import OptionParser

import sys
import re

import parsers
import checkers

#
# ----- classes ---------------------------------------------------------------
#

#
# forest
#

class c_forest:
  def __init__(self):
    self.program = None
    self.arguments = None
    self.options = None
    self.parser = None
    self.checker = None
    self.files = None
    self.summary = ""
    self.count = 0

#
# ----- functions -------------------------------------------------------------
#

#
# error()
#
def		error(f, string):
  sys.stderr.write("usage: forest.py [options]\n"
                   "\n"
                   "forest.py: " + string + "\n")
  sys.exit(42)



#
# summary()
#
def		summary(f):
  x = None

  if f.options.output_detail:
    x = f.summary

    f.summary = ""

    f.summary += "<forest version='" + VERSION + "' author='" +		\
                 AUTHOR + "' contributors='" + CONTRIBUTORS + "'>\n"

    if x:
      f.summary += "  <files>\n"
      f.summary += x
      f.summary += "  </files>\n"

    f.summary += "  <summary errors='" + str(f.count) + "'/>\n"
    f.summary += "</forest>\n"

  if f.options.output_pure:
    x = f.summary

    f.summary = ""

    f.summary += x
    f.summary += "[summary] " + str(f.count) + " errors.\n"



#
# output()
#
def		output(f):
  handle = None

  if f.options.output:
    try:
      handle = open(f.options.output, "w")
    except IOError:
      error(f, "error: unable to open the file \"" + f.options.output + "\".")

    handle.write(f.summary)
  else:
    sys.stdout.write(f.summary)



#
# process()
#
def		process(f, file):
  s = None
  c = None
  p = None

  try:
    p = parsers.parse(f.parser, file, f.options)
  except parsers.exceptions.ParserError:
    error(f, "error: unable to parse the file \"" + file + "\".")

  s, c = checkers.check(f.parser, f.checker, p, f.options)

  if s and c:
    f.summary += s
    f.count += c



#
# options()
#
s_parser = "--parser(?:=| +)([a-zA-Z0-9_]*)"
e_parser = re.compile(s_parser)

s_checker = "--checker(?:=| +)([a-zA-Z0-9_]*)"
e_checker = re.compile(s_checker)

def		options(f, arguments):
  optparse = OptionParser()
  options = None
  match = None
  o = None

  match = e_parser.search(" ".join(arguments))
  if match:
    f.parser = match.group(1)

  match = e_checker.search(" ".join(arguments))
  if match:
    f.checker = match.group(1)

  optparse.add_option("--parser",
                      action="store",
                      dest="parser",
                      metavar="PARSER",
                      type="string",
                      help="parser")

  optparse.add_option("--checker",
                      action="store",
                      dest="checker",
                      metavar="STYLE",
                      type="string",
                      help="checker")

  optparse.add_option("--output",
                      action="store",
                      dest="output",
                      metavar="FILE",
                      type="string",
                      help="output file")

  optparse.add_option("--output-pure",
                      action="store_true",
                      dest="output_pure",
                      help="output a pure summary")

  optparse.add_option("--output-detail",
                      action="store_true",
                      dest="output_detail",
                      help="output a detailed summary")

  o = parsers.options(f.parser, optparse)
  if o:
    optparse = o

  o = checkers.options(f.parser, f.checker, optparse)
  if o:
    optparse = o

  options, arguments = optparse.parse_args(arguments)

  return options, arguments



#
# main()
#
def		main():
  f = c_forest()
  optparse = None
  file = None

  f.program = sys.argv[0]
  f.arguments = sys.argv[1:]

  f.options, f.files = options(f, f.arguments)

  if not f.parser:
    error(f, "error: please specify a parser.")

  if not f.files:
    error(f, "error: please specify at least one file.")

  if f.checker and (not f.options.output_pure and not f.options.output_detail):
    error(f, "error: please specify an output format: pure or detailed.")

  if not f.checker and (f.options.output_pure or f.options.output_detail):
    error(f, "error: please specify a checker.")

  for file in f.files:
    process(f, file)

  summary(f)

  output(f)



#
# banner()
#
def		banner():
  sys.stderr.write("\n" +						\
                   "-------[" +						\
                   " forest " +						\
                   "]" +						\
                   "-------------------------------" +			\
                   "-------------------------------" +			\
                   "\n\n")



#
# entry point
#
if __name__ == "__main__":
  banner()
  main()
