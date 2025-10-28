#!/bin/bash

# Check if all header files are included in file libxml++/libxml++.h.

# For each file libxml++/*/<name>.h except libxml++/libxml++.h, check if
# libxml++/*/<name>.h is included in libxml++/libxml++.h.
# If the file is not included in libxml++/libxml++.h, search for inclusion
# in any of the files libxml++/*/<name>.h. Thus you can see if it's included
# indirectly via another header file.

# Some manual checking of libxml++/libxml++.h is usually necessary.
# Perhaps some header files shall not be included, such as <name>_private.h.
# Other header files shall perhaps be surrounded by #ifdef/#ifndef/#endif
# directives.

dir=libxml++
file=libxml++.h

for headerfile in $dir/*.h $dir/*/*.h
do
  if [ $headerfile != $dir/$file ]
  then
    h2="${headerfile//./\.}" # Replace each "." by "\."
    echo "=== $headerfile"
    grep -q  "^ *# *include  *<$h2>" $dir/$file
    if [ $? -ne 0 ]
    then
      echo "  Missing"
      grep "<$h2>" $dir/*.h $dir/*/*.h
    fi
  fi
done

exit 0
