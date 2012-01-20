#!/bin/sh

# Script for automatic generation of Makefile dependencies
# Receives list of .c files as arguments

echo "# Created by makedep.sh, do not edit" > Makefile.dep
echo "" >> Makefile.dep

for x in $@
do
  if ! $CPP $CPPFLAGS -E -MG -MM $x >> Makefile.dep ; then
    rm -f Makefile.dep
    exit 1
  fi
  echo -e "\t\$(MPICC) -c \$(CFLAGS) $x" >> Makefile.dep
done

echo "skampi: $OBJECTS"   >> Makefile.dep
echo -e "\t\$(MPICC) -o skampi $OBJECTS -lm" >> Makefile.dep
