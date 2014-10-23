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
set( ArachneCV_VERSION_MINOR 3 )

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
  ArachneCV.hpp
  camera.hpp camera.cpp
  image_analysis.hpp image_analysis.cpp
  targets.hpp targets.cpp
)

find_package(OpenCV REQUIRED)
target_link_libraries(ArachneCV ${OpenCV_LIBS})

install( TARGETS ArachneCV DESTINATION lib )
install( FILES "${PROJECT_BINARY_DIR}/ArachneCVConfig.h" ArachneCV.hpp camera.hpp image_analysis.hpp targets.hpp DESTINATION "include/ArachneCV" )

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
  camera.cpp
  image_analysis.cpp
  targets.cpp
)
swig_link_libraries( ArachneCV ${OpenCV_LIBRARIES} ${PYTHON_LIBRARIES} )

ENDIF( OUTPUT_PYTHON )
