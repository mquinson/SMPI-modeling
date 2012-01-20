#!/bin/bash -e

OFFICIAL_DISTRIBUTION=$1
shift
if [[ 0 != $OFFICIAL_DISTRIBUTION && 1 != $OFFICIAL_DISTRIBUTION ]] ; then
  echo "make_distribution: error: first parameter is '$OFFICIAL_DISTRIBUTION'"
  echo "       it should be 1 for a official distribution and 0 otherwise"
  exit 1
fi

SVN_FILES=`svn status --verbose $@ | egrep -v "^\?" | awk ' { print $NF; }'`

if [[ -n `svn status --verbose $SVN_FILES | egrep -v "^ "` ]] ; then
  echo "make_distribution: error: files do not represent a consistent revision number."
  echo "       can't build a distribution, please check in files or"
  echo "       revert them to a consistent revision number" 
  exit 1
fi

MAX_REVISION=`svn status --verbose $SVN_FILES | egrep "^ " | awk ' { print $1; }' | sort -n | tail -n 1`
if [[ -z $MAX_REVISION || -n `svn diff -r $MAX_REVISION $SVN_FILES`  ]] ; then
  echo "make_distribution: error: there are changes which are not checked in"
  exit 1
fi

MAX_REVISION=`svnversion -n .`
FORMATTED_MAX_REVISION=`echo "0000$MAX_REVISION" | sed 's@.*\([0-9][0-9][0-9][0-9]\)@\1@' `

# @@@ extract is not robust enough
MAJ_VERSION=`grep skampi_maj_version version.h | awk ' { print $5; }'`
MIN_VERSION=`grep skampi_min_version version.h | awk ' { print $5; }'`
SUB_RELEASE=`grep skampi_sub_release version.h | awk ' { print $5; }'`

if [[ 1 == $OFFICIAL_DISTRIBUTION ]] ; then
  skampidir=skampi-${MAJ_VERSION}.${MIN_VERSION}.${SUB_RELEASE}
else
  skampidir=skampi-${MAJ_VERSION}.${MIN_VERSION}.${SUB_RELEASE}-r${FORMATTED_MAX_REVISION}
fi

rm -rf $skampidir
mkdir $skampidir
for fname in $@; do
    mkdir -p $skampidir/`dirname $fname`
    cp -a $fname $skampidir/`dirname $fname`
done

sed -i -e "s/@svn_revision@/$MAX_REVISION/" $skampidir/version.h
sed -i -e "s/@maj_version@/$MAJ_VERSION/" $skampidir/README
sed -i -e "s/@min_version@/$MIN_VERSION/" $skampidir/README
sed -i -e "s/@sub_release@/$SUB_RELEASE/" $skampidir/README
sed -i -e "s/@svn_revision@/$MAX_REVISION/" $skampidir/README

sed -n "1,/# end of distributed part/p" < Makefile | grep -v -e "default:" > $skampidir/Makefile
touch -r Makefile $skampidir/Makefile

tar czf $skampidir.tar.gz $skampidir
rm -rf $skampidir

