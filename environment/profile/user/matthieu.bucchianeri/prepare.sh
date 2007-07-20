#!/bin/sh

if [ $# -ne 1 ]; then
    echo "usage: $0 spe"
    exit 1
fi

function get_fullname()
{
  yp="$1"
  onames="`echo $yp |cut -d':' -f 5`"
  ofirstname="`echo $onames |cut -d' ' -f 1`"
  oname="`echo $onames |cut -d' ' -f 2`"

  firstname=`echo $ofirstname | cut -c -1 | tr '[:lower:]' '[:upper:]'`
  firstname=${firstname}`echo $ofirstname | cut -c 2-`

  name=`echo $oname | cut -c -1 | tr '[:lower:]' '[:upper:]'`
  name=${name}`echo $oname | cut -c 2-`

  wikiname=`echo $firstname $name`
  echo $wikiname
}

id=1
for u in `cat groups.$1`;
do
  binom=`echo $u | cut -d':' -f 2`
  u=`echo $u | cut -d':' -f 1`
  yp1="`./ypcat |grep $u`"
  yp2="`./ypcat |grep $binom`"

  mkdir "$1$id"
  mkdir "$1$id"/sources
  mkdir "$1$id"/cheats
  mkdir "$1$id"/fingerprints

  echo `get_fullname "$yp1"` "<$u@epita.fr>" > "$1$id"/people
  echo `get_fullname "$yp2"` "<$binom@epita.fr>" >> "$1$id"/people

  id=$((id + 1))
done
