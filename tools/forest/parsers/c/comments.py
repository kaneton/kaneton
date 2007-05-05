##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/comments.py
##
## created       julien quintard   [tue sep  5 20:49:13 2006]
## updated       julien quintard   [sat sep 16 18:49:50 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the comments.
#
# either the comment is ending a line, in this case the comment is removed;
# or it is not, in this case the comment is replace by an empty comment:
# /*     */.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_comment = "(?: *)/\*(?:[^*]|[\n]|(?:\*+(?:[^*/]|[\n])))*\*+/"

e_comment_remove = re.compile("(" + s_comment + "\n" + ")",
                              re.MULTILINE)

e_comment_keep = re.compile("(" + s_comment + ")",
                            re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  comments = None
  remove = None
  keep = None
  c = None

  remove = e_comment_remove.findall(contents)

  for c in remove:
    contents = contents.replace(c, "\n", 1)

  keep = e_comment_keep.findall(contents)

  for c in keep:
    contents = contents.replace(c, parser.forge(parser.FORGE_STREAM,
                                                c,
                                                len(c)),
                                1)

  comments = remove + keep

  return comments, contents
