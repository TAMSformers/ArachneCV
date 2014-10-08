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

IF( OUTPUT_NATIVE )

add_library( ArachneCV
  camera_acv.cpp camera_acv.hpp
  camera_proxy.cpp camera_proxy.hpp
)

find_package(OpenCV REQUIRED)
target_link_libraries(ArachneCV ${OpenCV_LIBS})

install( TARGETS ArachneCV DESTINATION bin )
install( FILES "${PROJECT_BINARY_DIR}/ArachneCVConfig.h" camera_acv.hpp camera_proxy.hpp DESTINATION include )

ENDIF( OUTPUT_NATIVE )

IF( OUTPUT_PYTHON )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

find_package( SWIG REQUIRED )
include( ${SWIG_USE_FILE} )

find_package( PythonLibs )
include_directories( ${PYTHON_INCLUDE_PATH} )

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_PATH} )

set( CMAKE_SWIG_FLAGS "" )

set_source_files_properties( ArachneCV.i PROPERTIES CPLUSPLUS ON )
swig_add_module( ArachneCV python ArachneCV.i
  camera_proxy.cpp
  camera_acv.cpp
)
swig_link_libraries( ArachneCV ${OpenCV_LIBRARIES} ${PYTHON_LIBRARIES} )

ENDIF( OUTPUT_PYTHON )
