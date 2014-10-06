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

option( OUTPUT_NATIVE "Compile for use by C++ programs." ON )
#option( OUTPUT_CSHARP "Compile for use by C# programs." ON )
#option( OUTPUT_JAVA "Compile for use by Java programs." ON )
#option( OUTPUT_PERL "Compile for use by Perl programs." ON )
option( OUTPUT_PYTHON "Compile for use by Python programs." ON )

configure_file(
  "${PROJECT_SOURCE_DIR}/ArachneCVConfig.h.in"
  "${PROJECT_BINARY_DIR}/ArachneCVConfig.h"
)

include_directories( "${PROJECT_SOURCE_DIR}" )
include_directories( "${PROJECT_BINARY_DIR}" )

add_library( ArachneCV
  camera_acv.cpp camera_acv.hpp
  camera_proxy.cpp camera_proxy.hpp
)

find_package(OpenCV REQUIRED)
target_link_libraries(ArachneCV
    opencv_core
    opencv_imgproc
    opencv_highgui
)

install( TARGETS ArachneCV DESTINATION bin )
install( FILES "${PROJECT_BINARY_DIR}/ArachneCVConfig.h" camera_acv.hpp camera_proxy.hpp DESTINATION include )

#if( OUTPUT_PYTHON )
#find_package( SWIG REQUIRED )
#include( ${SWIG_USE_FILE} )

#find_package( PythonLibs )
#include_directories( ${PYTHON_INCLUDE_PATH} )
#endif( OUTPUT_PYTHON )
