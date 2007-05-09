#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...file/environment/behaviour/linux/init.py
#
# created       julien quintard   [sun dec 17 02:28:37 2006]
# updated       julien quintard   [wed may  9 14:48:58 2007]
#

#
# ---------- imports ----------------------------------------------------------
#

import env

#
# ---------- globals ----------------------------------------------------------
#

g_binaries = [ "bash", "gcc", "gmake", "rm", "ar", "ranlib", "ld",	\
               "nasm", "ln", "touch", "wc", "date", "tail", "tar",	\
               "pdflatex", "cp", "cat", "sed", "ls", "cpp", "mtools",	\
               "mcopy", "grep", "mkdir", "find", "xpdf", "mktemp",	\
               "mv", "lex", "script", "perl", "python" ]

#
# ---------- functions --------------------------------------------------------
#

#
# check()
#
# this function tries to locate the binaries needed by this machine profile.
#
def			check():
  binary = None

  # display some stuff.
  env.display(env.HEADER_OK, "  checking for needed binaries location")

  for binary in g_binaries:
    if not env.locate(binary):
      env.display(env.HEADER_ERROR, "    '" + binary +			\
                  "' is not present on your system")



#
# main()
#
# this function initializes the machine-specific development
# environment stuff.
#

def			main():
  # check on the presence of the fundamental binaries.
  check()

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
