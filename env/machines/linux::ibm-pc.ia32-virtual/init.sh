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

BINARIES="bash gcc gmake rm ar ranlib ld nasm ln touch wc date	\
          tail tar pdflatex cp cat sed ls cpp mtools mcopy grep	\
          mkdir find xpdf mktemp mv lex script perl python"

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
