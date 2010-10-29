#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/certificate.py
#
# created       julien quintard   [mon oct 25 20:01:03 2010]
# updated       julien quintard   [wed oct 27 13:10:06 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

from OpenSSL import crypto

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".crt"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a certificate request.
#
def                     Request(key,
                                digest = "md5",
                                **attributes):
  request = crypto.X509Req()

  subject = request.get_subject()

  for (k, v) in attributes.items():
    setattr(subject, k, v)

  request.set_pubkey(key)
  request.sign(key, digest)

  return request

#
# this function creates a certificate according to the given
# certificate request.
#
def                     Create(request,
                               issuer,
                               serial,
                               timestamp,
                               digest = "md5"):
  issuer_certificate = None
  issuer_key = None
  timestamp_notbefore = None
  timestamp_notafter = None

  (issuer_certificate, issuer_key) = issuer
  (timestamp_notbefore, timestamp_notafter) = timestamp

  certificate = crypto.X509()

  certificate.set_serial_number(serial)
  certificate.gmtime_adj_notBefore(timestamp_notbefore)
  certificate.gmtime_adj_notAfter(timestamp_notafter)
  certificate.set_issuer(issuer_certificate.get_subject())
  certificate.set_subject(request.get_subject())
  certificate.set_pubkey(request.get_pubkey())

  certificate.sign(issuer_key, digest)

  return certificate

#
# this function stores the given certificate on the file system.
#
def                     Store(path,
                              certificate):
  open(path,
       'w').write(crypto.dump_certificate(crypto.FILETYPE_PEM, certificate))
