#!/bin/sh

if [ $# -ne 2 ]; then
    echo "usage: $0 step spe"
    exit 1
fi

function get_wikiname()
{
  yp="$1"
  onames="`echo $yp |cut -d':' -f 5`"
  ofirstname="`echo $onames |cut -d' ' -f 1`"
  oname="`echo $onames |cut -d' ' -f 2`"

  firstname=`echo $ofirstname | cut -c -1 | tr '[:lower:]' '[:upper:]'`
  firstname=${firstname}`echo $ofirstname | cut -c 2-`

  name=`echo $oname | cut -c -1 | tr '[:lower:]' '[:upper:]'`
  name=${name}`echo $oname | cut -c 2-`

  wikiname=`echo $firstname$name |tr -d "-" |tr -d "'"`
  echo $wikiname
}

id=1
for u in `cat groups.$2`;
do
  binom=`echo $u | cut -d':' -f 2`
  u=`echo $u | cut -d':' -f 1`
  yp1="`./ypcat |grep $u`"

  wikiname=`get_wikiname "$yp1"`
  yp2="`./ypcat |grep $binom`"
  bi=`get_wikiname "$yp2"`

  for i in 9 8 7 6 5 4 3 2 1;
  do
    cp "$wikiname-$1-$i.tar.bz2" "$2$id"/sources/"`echo $1 | tr '[:upper:]' '[:lower:]'`".tar.bz2 2> /dev/null
    if [ "$?" -eq 0 ]; then
	break
    fi
  done

  id=$((id + 1))
done
