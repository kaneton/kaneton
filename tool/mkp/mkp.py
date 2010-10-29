#
# Makeprotos / mkp.py
#   Made by pwipwi <pwipwi@lse.epita.fr>
# This python script was created to
#   1) Give me an opportunity to practice python a bit :)
#   2) Replace old mkp.pl, which was sufficient, but a bit unreadable.
#
# usage:
#    ./mkp.py header1 header2 ...
#
#  For this script to work efficiently, you have a few things to do :
#       * Code with the Epita CSS, at least in function declaration.
#         This little script is far from being a C parser, so it will
#         exclusively recognize Epita CSS' functions declarations
#       * You have to end your prototypes section by a
#         /*
#          * eop
#          */
#          statement
#       * Last but not least, you need to have a VALID prototypes secion.
#         it looks like that
#         /*
#          * ---------- prototypes --------------------
#          *
#          *      ./relative/path/to/file/1
#          *      ./relative/path/to/file/2
#          *
#          *      ./et/caetera
#          */
#         Be sure to respect the spacing, as the script is especially not
#         tolerant with them (especially before the *).
#         You can have as much spaces as you want between the * and the file
#         names, though.
#
#  That should do the trick. If you ever encounter a problem which is most
#  likely a bug in that script, please send me a mail.
#

# System module
import sys
# OS specific module
import os
# Regular Expression module
import re


## --------- Classes ----------------------------------------------------------

class cl_functions:
  """ cl_functions : functions definitions
  This class is the one that shall hold all the function definitions
  we will have found in the diverse files.
  """
  def __init__(self):
    self.di_files = {} # initialize file dictionnary
    self.beginning = []
    self.ending = []
    self.filenames = []

  def add_filename(self, str_filename):
    self.di_files[str_filename] = []

  def add_function(self, str_filename, li_functiondecl):
    self.di_files[str_filename].append(li_functiondecl)

  def purge(self):
    self.di_files = {}



## --------- functions ---------------------------------------------------------

##
## error
##
def fn_error(string, code):
  """Throw an Error
  Print an error and exit the program with the corresponding code
  """
  print(sys.argv[0] + ':' + string)
  sys.exit(code)

##
## readprotosfiles
##
def fn_readprotosfiles(fp, functions):
  """Readprotos files
  Read the file names of the files that shall be processed
  """
  filelist = []
  line = fp.readline()
  while line != '' and not re.match(' \* -+ prototypes -+', line):
    functions.beginning.append(line)
    line = fp.readline()
  if line == '':
    fn_error('this file doesn\'t contain a valid prototype section', 3)
  # iterate until the end of the prototypes section
  endre = re.compile('.*\*/')
  filere = re.compile('\s*\*+\s+[./_a-zA-Z0-9]+')
  filenamere = re.compile('[-./_a-zA-Z0-9]+')
  eopre = re.compile('\s*\*+\s+eop')
  while line != '' and not endre.match(line):
    functions.beginning.append(line)
    if filere.match(line):
      line = filenamere.search(line).group()
      filelist.append(line)
    line = fp.readline()
  functions.beginning.append(line)
  while line != '' and not eopre.match(line):
    line = fp.readline()
  while line != '' and not endre.match(line):
    line = fp.readline()
  if line == '':
    fn_error("no eop in" + fp.filename, 3)
  while line != '':
    line = fp.readline()
    functions.ending.append(line)
  del filere
  del endre
  del filenamere
  return filelist

##
## processfile
##
def fn_processfile(filename, functions):
  """Process files
  returns a function list from the given file
  """
  inside_funcion = 0
  inside_comment = 0
  currentlist = []

  try:
    fp = open(filename, 'r')
  except IOError:
    fn_error("unable to open" + filename, 4)
  functionre = re.compile('^[a-zA-Z0-9_](([\*\w]*)\s+\**)+\w+\(') # function declaration
  commentre = re.compile('/\*') # comment start
  endcomre = re.compile('\*/') # comment end
  staticre = re.compile('static') # static keyword
  for line in fp.readlines():
    matched = functionre.search(line)
    commatched = commentre.search(line)
    endcommatched = endcomre.search(line)
    if commatched:
      inside_comment = inside_comment + 1
    if endcommatched:
      inside_comment = inside_comment - 1
    if matched and not inside_comment:
      inside_funcion = 1
    if inside_funcion:
      static_match = staticre.search(line)
      if re.search('\)\n', line) and not static_match:
	line = re.match("(.*)$", line).group(0)
	line = line + ';\n'
	currentlist.append(line)
        functions.add_function(filename, currentlist)
	currentlist = []
	inside_funcion = 0
	continue
      else:
	if not static_match:
	  currentlist.append(line)
	else:
	  inside_funcion = 0
	continue
  fp.close()


##
##
##
def fn_write_header(functions, filename):
  """Write the header

  Write the function declarations to the header files
  """
  try:
    fp = open(filename, 'w')
  except IOError:
    fn_error(filename + " is not writeable")
  fp.writelines(functions.beginning)
  for file in functions.filenames:
    fp.write('\n/*\n * ' + file + '\n */\n\n')
    for func in functions.di_files[file]:
      fp.writelines(func)
      fp.write('\n')
  fp.write('\n/*\n * eop\n */\n')
  fp.writelines(functions.ending)
  fp.close()

##
## mkprotos
##
def fn_mkprotos(path):
  """ Make protos
  This function calls all the others. It proceeds in that order:
    first, it calls fn_readprotos to get a list of the files to parse
    and recover prototypes.
    then, for each of those files, it calls fn_processfile, which
    parses the file and gets the prototypes.
    finally, it calls fn_write_header to actually write the header
  """
  functions = cl_functions()
  currentpwd = os.getcwd()
  filename = ''

  if (path.count('/') >= 1): # is it
    filename = re.sub('.*/', '', path) # splits the filename from the path
    dirname = re.sub('/[^/]*$', '', path) # splits the directory name from the path
    try:
      os.chdir(dirname)
    except OSError:
      fn_error(dirname + ', no such file or directory !', 1)
  else:
    filename = path
  try:
    fp = open(filename, 'r')
  except IOError:
    fn_error(filename + ', no such file', 2)
  functions.filenames = fn_readprotosfiles(fp, functions)
  for file in functions.filenames:
    functions.add_filename(file)
    fn_processfile(file, functions)
  fp.close()
  fn_write_header(functions, filename)
  os.chdir(currentpwd)
  return functions


##
## \o/ Main function \o/
##
args = sys.argv[1:]
if len(args) == 0:
  fn_error('Give at least 1 argument, you silly man !', 3)
for file in args:
  functions = fn_mkprotos(file)
