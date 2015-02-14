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

set_source_files_properties(ArachneCV.i PROPERTIES CPLUSPLUS ON)
swig_add_module(ArachneCV_CSharp csharp ArachneCV.i
  ../../camera.cpp
  ../../targets.cpp
  ../../analysis/analysis_helper.cpp
  ../../analysis/bins.cpp
  ../../analysis/noodles.cpp
  ../../analysis/scoring_zones.cpp
  ../../analysis/totes.cpp
  ../../analysis/totes_depth.cpp
)
set_target_properties(ArachneCV_CSharp PROPERTIES OUTPUT_NAME ArachneCV)

swig_link_libraries(ArachneCV_CSharp ${OpenCV_LIBRARIES})
swig_link_libraries(ArachneCV freenect2 usb-1.0)

