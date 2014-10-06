#
# Arachne CV
#
# Written by David Hashe and Jacob Brunson
#
#
#

cmake_minimum_required( VERSION 2.6 )

project( ArachneCV )

set( ArachneCV_VERSION_MAJOR 0 )
set( ArachneCV_VERSION_MINOR 2 )

set( BUILD_SHARED_LIBS ON )

option( OUTPUT_NATIVE "Compile for use by C++ programs." ON)
#option( OUTPUT_CSHARP "Compile for use by C# programs." ON )
#option( OUTPUT_JAVA "Compile for use by Java programs." ON )
#option( OUTPUT_PERL "Compile for use by Perl programs." ON )
option( OUTPUT_PYTHON "Compile for use by Python programs." OFF )

configure_file(
  "${PROJECT_SOURCE_DIR}/ArachneCVConfig.h.in"
  "${PROJECT_BINARY_DIR}/ArachneCVConfig.h"
)

include_directories( "${PROJECT_SOURCE_DIR}" )
include_directories( "${PROJECT_BINARY_DIR}" )

if( OUTPUT_NATIVE )

add_library( ArachneCV
  camera_acv.cpp camera_acv.hpp
  camera_proxy.cpp camera_proxy.hpp
)

target_link_libraries( ArachneCV
  libopencv_core.so
  libopencv_highgui.so
  libopencv_imgproc.so
)

install( TARGETS ArachneCV DESTINATION bin )
install( FILES "${PROJECT_BINARY_DIR}/ArachneCVConfig.h" camera_acv.hpp camera_proxy.hpp DESTINATION include )

endif( OUTPUT_NATIVE )

if( OUTPUT_PYTHON )

find_package( SWIG REQUIRED )
include( ${SWIG_USE_FILE} )

find_package( PythonLibs )
include_directories( ${PYTHON_INCLUDE_PATH} )

set( CMAKE_SWIG_FLAGS "" )

set_source_files_properties( ArachneCV.i PROPERTIES CPLUSPLUS ON )
set_source_files_properties( ArachneCV.i PROPERTIES SWIG_FLAGS "-includeall")
swig_add_module( ArachneCV python ArachneCV.i
  camera_proxy.cpp
  camera_proxy.hpp
  camera_acv.cpp
  camera_acv.hpp
)
swig_link_libraries( ArachneCV ${PYTHON_LIBRARIES} )

endif( OUTPUT_PYTHON )
