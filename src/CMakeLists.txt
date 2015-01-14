#
# Arachne CV
#
# Written by David Hashe and Jacob Brunson
#
#
#

set(BUILD_SHARED_LIBS ON)

option(OUTPUT_NATIVE "Compile for use by C++ programs." ON)
#option(OUTPUT_CSHARP "Compile for use by C# programs." ON)
#option(OUTPUT_JAVA "Compile for use by Java programs." ON)
#option(OUTPUT_PERL "Compile for use by Perl programs." ON)
option(OUTPUT_PYTHON "Compile for use by Python programs.")

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_BINARY_DIR}/src")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -O2 -std=c++11")

if(OUTPUT_NATIVE)

add_library(ArachneCV
  ArachneCV.hpp
  camera.hpp camera.cpp
  targets.hpp targets.cpp
  analysis/analysis_helper.hpp
  analysis/analysis_helper.cpp
  analysis/analysis.hpp
  analysis/bins.cpp
  analysis/noodles.cpp
  analysis/scoring_zones.cpp
  #analysis/totes.cpp
)

find_package(OpenCV REQUIRED)
target_link_libraries(ArachneCV ${OpenCV_LIBS})

install(TARGETS ArachneCV DESTINATION lib)
install(FILES ArachneCV.hpp camera.hpp targets.hpp DESTINATION "include/ArachneCV")
install(FILES analysis/analysis.hpp DESTINATION "include/ArachneCV/analysis")

endif(OUTPUT_NATIVE)

if(OUTPUT_PYTHON)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS "")

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
  camera.cpp
  targets.cpp
  analysis/analysis_helper.cpp
  analysis/bins.cpp
  analysis/noodles.cpp
  analysis/scoring_zones.cpp
  #analysis/totes.cpp
)
swig_link_libraries(ArachneCV ${OpenCV_LIBRARIES} ${PYTHON_LIBRARIES})

#add_dependencies(ArachneCV patch)

if(${UNIX})
execute_process(COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()"
                OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
install(TARGETS _ArachneCV DESTINATION ${PYTHON_SITE_PACKAGES})
install(FILES ${PROJECT_BINARY_DIR}/src/ArachneCV.py DESTINATION ${PYTHON_SITE_PACKAGES})
endif(${UNIX})

endif(OUTPUT_PYTHON)
