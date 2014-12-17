# Arachne CV
#
# Written by David Hashe
#
# SWIG does not allow C++ function overloading when one of the arguments is a
# Python list, as in acv::Camera::Camera(). Fortunately, this can be corrected
# through a straightforward modification of the generated wrapper file.
#
# This script is called by cmake_fix_makefile during building to modify the
# generated wrapper to ignore the Python list.
#

import string as s

wrapper_file = open("src/ArachneCVPYTHON_wrap.cxx", "r")
wrapper_str = wrapper_file.read()
wrapper_file.close()

for string in ["_wrap_new_WarpCamera(", "_wrap_new_DepthCamera("]:
    # Navigate to place to insert command
    index = s.find(wrapper_str, string)

    # First instance
    index = s.find(wrapper_str,"SWIG_ConvertPtr", index)
    for i in xrange(2):
        index = s.find(wrapper_str,"\n", index+1)
    first_index = index

    # Second instance
    index = s.find(wrapper_str,"SWIG_ConvertPtr", index)
    for i in xrange(2):
        index = s.find(wrapper_str,"\n", index+1)
    second_index = index

    # Split file into three strings at those points
    begin_str = wrapper_str[:first_index]
    middle_str = wrapper_str[first_index:second_index]
    end_str = wrapper_str[second_index:]

    # Combine with new command in middle
    wrapper_str = begin_str + "\n      _v = 1;" + \
                  middle_str + "\n      _v = 1;" + end_str

wrapper_file = open("src/ArachneCVPYTHON_wrap.cxx", "w")
wrapper_file.write(wrapper_str)
wrapper_file.close()

