#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/key.py
#
# created       julien quintard   [mon oct 25 20:11:55 2010]
# updated       julien quintard   [wed oct 27 13:11:59 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

from OpenSSL import crypto

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".key"

#
# ---------- functions --------------------------------------------------------
#

#
# this function generates a cryptographic key.
#
def                     Generate(type = crypto.TYPE_RSA,
                                 length = 2048):
  key = crypto.PKey()

  key.generate_key(type, length)

  return key

#
# this function stores a key.
#
def                     Store(path,
                              key):
  open(path,
       'w').write(crypto.dump_privatekey(crypto.FILETYPE_PEM, key))
