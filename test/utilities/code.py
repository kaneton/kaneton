#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/utilities/code.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [sat oct 30 11:18:05 2010]
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
import ktp

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
  code = ktp.code.Generate()

  # store the code.
  ktp.code.Store(env._TEST_STORE_CODE_DIR_ + "/server" + ktp.code.Extension,
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
