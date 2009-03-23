#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/util/code.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [sun mar 22 21:19:41 2009]
#

#
# ---------- information ------------------------------------------------------
#
# this program generates the server's private code.
#

#
# ---------- packages ---------------------------------------------------------
#

import env
import ktc

#
# ---------- functions --------------------------------------------------------
#

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
# main()
#
# this is the main function.
#
def                     main():
  code = None

  # display.
  env.display(env.HEADER_OK,
              "generating the server's code",
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # display a warning.
  warning()

  # generate a code.
  code = ktc.Code()

  # store the code.
  ktc.Store(env._TEST_STORE_CODE_DIR_ + "/server",
            code)

  # display.
  env.display(env.HEADER_OK,
              "server code successfuly generated and stored",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
