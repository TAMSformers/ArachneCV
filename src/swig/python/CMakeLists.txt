#
# Arachne CV
#
# Written by David Hashe and Jacob Brunson
#
#
#

find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

#configure_file(
#  "${PROJECT_SOURCE_DIR}/src/cmake_fix_makefile.py"
#  "${PROJECT_BINARY_DIR}/src/cmake_fix_makefile.py"
#)
#configure_file(
#  "${PROJECT_SOURCE_DIR}/src/cmake_fix_wrapper.py"
#  "${PROJECT_BINARY_DIR}/src/cmake_fix_wrapper.py"
#)
#add_custom_target(patch
#  COMMAND python cmake_fix_makefile.py
#)

set_source_files_properties(ArachneCV.i PROPERTIES CPLUSPLUS ON)
swig_add_module(ArachneCV python ArachneCV.i
  ../../camera.cpp
  ../../targets.cpp
  ../../analysis/analysis_helper.cpp
  ../../analysis/bins.cpp
  ../../analysis/noodles.cpp
  ../../analysis/scoring_zones.cpp
  ../../analysis/totes.cpp
)
swig_link_libraries(ArachneCV ${OpenCV_LIBRARIES} ${PYTHON_LIBRARIES})

#add_dependencies(ArachneCV patch)

if(${UNIX})
execute_process(COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()"
                OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
install(TARGETS _ArachneCV DESTINATION ${PYTHON_SITE_PACKAGES})
install(FILES ${PROJECT_BINARY_DIR}/src/swig/python/ArachneCV.py DESTINATION ${PYTHON_SITE_PACKAGES})
endif(${UNIX})

