#
# This is ArachneCV, a computer vision library for the FIRST
# Robotics Competition. Source hosted at
# https://github.com/tamsformers/ArachneCV
#
# Copyright 2014, 2015 by David Hashe and Jacob Brunson.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation - version 3 or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License in file COPYING for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program; if not, write to the Free
# Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02111, USA.
#

set(BUILD_SHARED_LIBS ON)

option(OUTPUT_NATIVE "Compile for use by C++ programs." ON)
#option(OUTPUT_CSHARP "Compile for use by C# programs." OFF)
#option(OUTPUT_JAVA "Compile for use by Java programs." ON)
#option(OUTPUT_PERL "Compile for use by Perl programs." ON)
option(OUTPUT_PYTHON "Compile for use by Python programs." ON)

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_BINARY_DIR}/src")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -O2 -std=c++11")

###############################################################################
#
# C/C++
#
###############################################################################
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
  analysis/totes.cpp
)

find_package(OpenCV REQUIRED)
target_link_libraries(ArachneCV ${OpenCV_LIBS})

target_link_libraries(ArachneCV freenect2)

install(TARGETS ArachneCV DESTINATION lib)
install(FILES ArachneCV.hpp camera.hpp targets.hpp DESTINATION "include/ArachneCV")
install(FILES analysis/analysis.hpp DESTINATION "include/ArachneCV/analysis")

endif(OUTPUT_NATIVE)

###############################################################################
#
# All SWIG Bindings
#
###############################################################################
if(OUTPUT_CSHARP OR OUTPUT_PYTHON)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_PATH})

target_link_libraries(ArachneCV freenect2)

set(CMAKE_SWIG_FLAGS "")

endif(OUTPUT_CSHARP OR OUTPUT_PYTHON)

###############################################################################
#
# CSharp
#
###############################################################################
if(OUTPUT_CSHARP)

add_subdirectory(swig/csharp)

endif(OUTPUT_CSHARP)

###############################################################################
#
# Python
#
###############################################################################
if(OUTPUT_PYTHON)

add_subdirectory(swig/python)

endif(OUTPUT_PYTHON)
