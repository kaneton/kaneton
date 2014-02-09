##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/functions_parser.py
##
## created       julien quintard   [sat sep  9 21:35:58 2006]
## updated       julien quintard   [sun sep 17 09:39:25 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the functions' body parts.
#
# the 'e' variable represents the 'c_epita' checker main variable, the 'z'
# variable contains everything necessary for the functions checking while
# the 'p' variable describes the current function checking process.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parsers

import checker
import classes
import errors
import common

import functions


 #####   ######   ####   ######  #    #  #####
 #    #  #       #    #  #        #  #   #    #
 #    #  #####   #       #####     ##    #    #
 #####   #       #  ###  #         ##    #####
 #   #   #       #    #  #        #  #   #
 #    #  ######   ####   ######  #    #  #

#
# function
#

s_function = "^(?P<arg_align>(?P<funcname_align>(?P<lead_spce> *)"                               \
             "([\w_]+\s+)*?"                                           \
             "([\w_]+(?P<wrong_star>\*))?"                                                \
             "([\*\s\b])*?)\**"                                                 \
             "(?P<funcname>[\w_]+)(?P<incorrect_spce>\s*)"             \
             "\()"                                                              \
             "(?P<arguments>"                                                   \
               ".*?"                                                            \
             ")"                                                                \
             "\)\s*\{ *\n"                                                      \
             "(?P<declarations>"                                                \
               "( *(?!return|do)([\w_]+\s+)+[\*\s]*[a-zA-Z_][\w_]*.*?;( *\n)*|" \
               " *(?!return|do)([\w_]+\s+)+\([\*\s]*[a-zA-Z_][\w_]*\s*\)\s*\(.*?;( *\n)*)*"       \
             ")"                                                                \
             "(?P<body>"                                                        \
             ".*"                                                               \
             ")"

e_function = re.compile(s_function, re.DOTALL)


# s_binop = "(?P<prec_spce>\s*)"                \
#           "(?P<op><<=|>>=|\+\+|--|<=|->|>=|==|!=|\|\||<<|>>|\*=|/=|%=|\+=|-=|&=|\|=|\^=|\+|/|\%|&|~|\^|!|<|>|=|\||\*|-|\.)" \
#           "(?P<next_spce>\s*)"
s_binop = "(?P<unop_before>--|\+\+)"            \
          "(?P<prec_spce>\s*)"                \
          "(?P<op><<=|>>=|\+\+|--|<=|->|>=|==|!=|\|\||<<|>>|\*=|/=|%=|\+=|-=|&=|\|=|\^=|\+|/|\%|&|~|\^|!|<|>|=|\||\*|-|\.)" \
          "(?P<next_spce>\s*)"
e_binop = re.compile(s_binop, re.MULTILINE)

s_comma = "(?P<prev_spce>\s*),(?P<next_spce>\s*)"
e_comma = re.compile(s_comma, re.MULTILINE)

s_keywords = "[^\w](?P<keyword>if|for|while|switch|return|sizeof)(?P<spce>\s*)\("
e_keywords = re.compile(s_keywords)

s_keywords_error = "[^\w_](?P<keyword>return|sizeof)(?P<spaces>\s+)(?P<error>.)"
e_keywords_error = re.compile(s_keywords_error)

s_function_call = "(?P<fname>[a-zA-Z_][a-zA-Z0-9_]*)(?P<spaces>\s*)\("
e_function_call = re.compile(s_function_call)

s_semi = "(?P<anything_prev>[^\s])(?P<prev_spce>\s*);(?P<next_spce>\s*)"
e_semi = re.compile(s_semi, re.MULTILINE)

s_unop = "[^a-zA-Z0-9_](?P<prec_space>\s*)"                 \
         "(?P<op>\+\+|--)"           \
         "(?P<follow_space>\s*)"
e_unop = re.compile(s_unop)

s_cast = "(?P<sizeof>(sizeof|))\s*" \
         "\(((const\s*)?([a-zA-Z_][a-zA-Z0-9_]*\s*)"     \
         "\*+\s*|"                            \
         "\s*(void|int|char|float|size_t|double|long)\s*)\)"
e_cast = re.compile(s_cast)

s_bracket_status = "([^\s]+\s*([{}]).*|.*([{}])\s*[^\s]+)"
e_bracket_status = re.compile(s_bracket_status)

 ######  #    #  #    #   ####    #####     #     ####   #    #   ####
 #       #    #  ##   #  #    #     #       #    #    #  ##   #  #
 #####   #    #  # #  #  #          #       #    #    #  # #  #   ####
 #       #    #  #  # #  #          #       #    #    #  #  # #       #
 #       #    #  #   ##  #    #     #       #    #    #  #   ##  #    #
 #        ####   #    #   ####      #       #     ####   #    #   ####

#
# parse()
#
# this function tries to locate the different function's parts.
#
def		parse(e, z, error):
  global str


  p = functions.c_function()
  match = None


  match = e_function.match(error.code)
  if not match:
    errors.add(e, error, errors.ERRORS_FATAL,
               "the function format is absolutely not understandable.\n")
    return None

  p.name = match.group('funcname')

  error.name = p.name
  p.arguments = match.group('arguments')

  p.a_arguments = len(match.group('arg_align'))

  p.a_declarations = len(match.group('funcname_align'))

  p.declarations = match.group('declarations')

  p.body = match.group('body')

  if len(match.group('incorrect_spce')) > 0:
    errors.add(e, error, errors.ERRORS_STYLE,
                 "function declaration MUST NOT contain spaces before (\n")

###########################################

  zone = p.declarations + p.body

  common.check_id_validity(e, error, p.name)

  if match.group('wrong_star'):
    errors.add(e, error, errors.ERRORS_STYLE,
               "* MUST NOT be next to the type name\n")

  if len(match.group('lead_spce')) != 0:
    errors.add(e, error, errors.ERRORS_STYLE,
               "function return type must begin on the first column\n")

  # ,
  for m in e_comma.finditer(zone):
    if len(m.group('next_spce')) != 1:
      if len(m.group('next_spce')) > 0 and m.group('next_spce')[0] != '\n':
        errors.add(e, error, errors.ERRORS_STYLE,
                 ", MUST be followed by an only whitespace\n")
    if len(m.group('prev_spce')) > 0:
      errors.add(e, error, errors.ERRORS_STYLE,
                 ", MUST NOT have whitespaces before it\n")

  # ;
  indent = 2
  for line in p.body.split('\n'):
    if e_bracket_status.search(line):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "{ or } MUST be on their own line\n")

    for m in e_function_call.finditer(line):
      name = m.group('fname')
      if name not in ['if', 'for', 'while', 'sizeof', 'return', 'switch', 'case']:
        if len(m.group('spaces')) > 0:
          errors.add(e, error, errors.ERRORS_STYLE,
                     "function calls MUST NOT have spaces between the name and the (\n")

    cast = e_cast.search(line)
    if cast:
      found = False
      if e.options.cast_ignore:
        for func_pattern in e.options.cast_ignore:
          if re.search(func_pattern, line):
            found = True
            break
      if not found and not cast.group('sizeof'):
        errors.add(e, error, errors.ERRORS_STYLE,
                   "explicit cast is forbidden by the CSS\n")

  for line in zone.split('\n'):
    line += '\n'
    ct = line.count(';')
    oper = e_keywords.search(line)


    for m in e_semi.finditer(line):
      if len(m.group('prev_spce')) > 0 and m.group('anything_prev') != '':
        errors.add(e, error, errors.ERRORS_STYLE,
                   "; MUST NOT have whitespaces before it\n")

    if oper and oper.group('keyword') == 'for' and ct >= 2:
      continue
    if ct > 2:
      errors.add(e, error, errors.ERRORS_STYLE,
                 "Only one instruction is allowed per line\n")

#     for m in e_semi.finditer(line):
#       if len(m.group('prev_spce')) > 0:
#         errors.add(e, error, errors.ERRORS_STYLE,
#                    "; MUST NOT have whitespaces before it\n")


  for m in e_keywords_error.finditer(zone):
    if m.group('error') != '(' and m.group('error') != ';':
      errors.add(e, error, errors.ERRORS_STYLE,
                 "%s keyword MUST have its argument between parenthesis\n" % m.group('keyword'))

  # keywords
  for m in e_keywords.finditer(zone):
    if len(m.group('spce')) == 1 and m.group('spce')[0] == ' ':
      pass # it's what we want
    else:
      errors.add(e, error, errors.ERRORS_STYLE,
                 "%s keyword MUST be followed by a whitespace\n" % m.group('keyword'))

  # Most of the binary operators (regexp limit some tests)
  for m in e_binop.finditer(zone):
    op = m.group('op')
    if op == '-' or op == '&' or op == '*':
      pass ## FIXME : How to parse operators that are both unaries and binaries with simple regexp ?
    elif op == '!' or op == '~':
      if len(m.group('next_spce')) > 0:
        errors.add(e, error, errors.ERRORS_STYLE,
                   "operator %s must have exactly zero space on the right\n" % op)
    elif op == '.' or op == '->':
      if len(m.group('prec_spce')) > 0:
        errors.add(e, error, errors.ERRORS_STYLE,
                   "operator %s must have exactly zero space on the left\n" % op)
      if len(m.group('next_spce')) > 0:
        errors.add(e, error, errors.ERRORS_STYLE,
                   "operator %s must have exactly zero space on the right\n" % op)
    elif op == '++' or op == '--':
      pass
    else:
      if len(m.group('prec_spce')) != 1: # FIXME manque le test de si
                                         # c'est une espace
        errors.add(e, error, errors.ERRORS_STYLE,
                   "operator %s must have exactly one space on the left\n" % op)
      if (len(m.group('next_spce')) > 1 and m.group('next_spce')[0] != '\n') or len(m.group('next_spce')) == 0:
        errors.add(e, error, errors.ERRORS_STYLE,
                   "operator %s must have exactly one space on the right or a newline\n" % op)

#######################################################

  if match.group(2):
    z.locals += 1
  else:
    z.exports += 1

    if z.exports > functions.FUNCTIONS_EXPORT_LIMIT:
      errors.add(e, error, errors.ERRORS_STYLE,
                 "the file contains more than " +			\
                 str(functions.FUNCTIONS_EXPORT_LIMIT) +		\
                 " exported \n")

  if (z.locals + z.exports) > functions.FUNCTIONS_FILE_LIMIT:
    errors.add(e, error, errors.ERRORS_STYLE,
               "the file contains more than " +				\
               str(functions.FUNCTIONS_FILE_LIMIT) + " functions.\n")

  return p
