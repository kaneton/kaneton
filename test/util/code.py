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
# updated       julien quintard   [mon apr 20 03:28:48 2009]
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
