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
set( ArachneCV_VERSION_MINOR 1 )

set( BUILD_SHARED_LIBS 1 )

configure_file(
  "${PROJECT_SOURCE_DIR}/ArachneCVConfig.h.in"
  "${PROJECT_BINARY_DIR}/ArachneCVConfig.h"
)

include_directories( "${PROJECT_SOURCE_DIR}" )
include_directories( "${PROJECT_BINARY_DIR}" )

#add_library( ArachneCV SHARED overhead.cxx )
#
add_library( ArachneCV overhead.cpp overhead.hpp )
target_link_libraries( ArachneCV
  libopencv_core.so
  libopencv_highgui.so
  libopencv_imgproc.so
)

install( TARGETS ArachneCV DESTINATION bin )
install( FILES "${PROJECT_BINARY_DIR}/ArachneCVConfig.h" overhead.hpp DESTINATION include )
