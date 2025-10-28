#!/bin/bash

# Compile each header file, one at a time.
# The compiler will notice if a header file does not include all other header
# files that it depends on.

# Usage: testheaders.sh <pkg>
# <pkg> is the name of the package, given to pkg-config, e.g. libxml++-5.0.

function usage() {
  echo "Usage: $0 <pkg>"
  echo "Example: $0 libxml++-5.0"
  exit 1
}

# Compiler, default: CXX=g++
if test "x$CXX" = x
then
  CXX=g++
fi

# Extra compiler flags, default: CXXFLAGS=-std=c++17
if test "x$CXXFLAGS" = x
then
  CXXFLAGS=-std=c++17
fi

# Directories to include in CFLAGS.
idirs="-I."

# Package name
if [ $# -lt 1 ]
then
  echo "No package name"
  usage
fi
pkg="$1"

CFLAGS="$idirs `pkg-config --cflags $pkg`"
if [ $? -ne 0 ]
then
  echo "pkg-config failed"
  usage
fi
echo CXX=$CXX, CXXFLAGS=$CXXFLAGS
echo CFLAGS=$CFLAGS

# Compile the header files
for headerfile in libxml++/*.h libxml++/*/*.h
do
  echo "=== $headerfile"
  $CXX -c -x c++ $CXXFLAGS -o /dev/null $headerfile $CFLAGS
done
