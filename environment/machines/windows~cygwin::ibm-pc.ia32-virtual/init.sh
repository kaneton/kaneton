## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/machines/gnu-unix/init.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       julien quintard   [fri jul 14 04:14:08 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run into the directory: env/machines/linux/
#
# this script installs operating system and architecture specific
# stuff.
#

#
# ---------- dependencies -----------------------------------------------------
#

source			.env.sh

#
# ---------- globals ----------------------------------------------------------
#

BINARIES="bash.exe gcc.exe make.exe rm.exe ar.exe ranlib.exe	\
          ld.exe nasm.exe ln.exe touch.exe wc.exe date.exe	\
          tail.exe tar.exe pdflatex.exe cp.exe cat.exe sed.exe	\
          ls.exe cpp.exe mtools.exe mcopy.exe grep.exe		\
          mkdir.exe find.exe xpdf.exe mktemp.exe mv.exe		\
          flex.exe script.exe perl.exe python.exe"

#
# ---------- functions --------------------------------------------------------
#

#
# CHECK
#
# checks whether the binaries used to compile the kaneton kernel are
# present on the system.
#
check()
{
  local needless
  local b

  display "   checking binaries" "+"

  for b in ${BINARIES} ; do
    needless=$(locate "${b}")

    if [ $? -ne 0 ] ; then
      display "   binary '${b}' not present on your system" "!"
    fi
  done
}

#
# ---------- entry point ------------------------------------------------------
#

# checks the binaries.
check
