# Arachne CV
#
# Written by David Hashe
#
# SWIG does not allow C++ function overloading when one of the arguments is a
# Python list, as in acv::Camera::Camera(). Fortunately, this can be corrected
# through a straightforward modification of the generated wrapper file.
#
# This script is called by make at build time to modify the Makefile to call
# another script to modify the generated wrapper to fix the Python list issue.
#
# So yeah.
#

import string as s

makefile_file = open("CMakeFiles/_ArachneCV.dir/build.make", "r")
makefile_str = makefile_file.read()
makefile_file.close()

# Navigate to place to insert command
index = s.find(makefile_str, "PYTHON_wrap.cxx:")
for i in xrange(6):
    index = s.find(makefile_str,"\n", index+1)

# Split file into two strings at that point
begin_str = makefile_str[:index]
end_str = makefile_str[index:]

# Combine with new command in middle
makefile_str = begin_str + "\tpython cmake_fix_wrapper.py\n" + end_str

makefile_file = open("CMakeFiles/_ArachneCV.dir/build.make", "w")
makefile_file.write(makefile_str)
makefile_file.close()
