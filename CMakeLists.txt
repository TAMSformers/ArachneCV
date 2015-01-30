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

cmake_minimum_required(VERSION 2.6)

project(ArachneCV)

set(ArachneCV_VERSION_MAJOR 0)
set(ArachneCV_VERSION_MINOR 3)

add_subdirectory(src)
add_subdirectory(tests)
