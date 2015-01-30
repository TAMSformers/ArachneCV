/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * This is ArachneCV, a computer vision library for the FIRST        *
 * Robotics Competition. Source hosted at                            *
 * https://github.com/tamsformers/ArachneCV                          *
 *                                                                   *
 * Copyright 2014, 2015 by David Hashe and Jacob Brunson.            *
 *                                                                   *
 * This program is free software; you can redistribute it and/or     *
 * modify it under the terms of the GNU General Public License as    *
 * published by the Free Software Foundation - version 3 or          *
 * (at your option) any later version.                               *
 *                                                                   *
 * This program is distributed in the hope that it will be useful,   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 * GNU General Public License in file COPYING for more details.      *
 *                                                                   *
 * You should have received a copy of the GNU General Public         *
 * License along with this program; if not, write to the Free        *
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,       *
 * Boston, MA 02111, USA.                                            *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

%module ArachneCV

%{
#include "camera.hpp"
#include "targets.hpp"

/* SWIG doesn't recognize Target as part of acv, for some reason. */
using acv::Target;
%}

%include "arrays_csharp.i"
CSHARP_ARRAYS( double, double )
%typemap(out) double INPUT[] "return $imcall;"
/*typemap(cstype) double OUTPUT[] " "*/
%apply double INPUT[] {double coords[3]}

/*%typemap(ctype)   std::vector<acv::Target> OUTPUT "" {

}

%typemap(cstype)  CTYPE OUTPUT[] "CSTYPE[]"
%typemap(imtype, inattributes="[Out, MarshalAs(UnmanagedType.LPArray)]") CTYPE OUTPUT[] "CSTYPE[]"
%typemap(csin)    CTYPE OUTPUT[] "$csinput"

%typemap(in)      CTYPE OUTPUT[] "$1 = $input;"
%typemap(freearg) CTYPE OUTPUT[] ""
%typemap(argout)  CTYPE OUTPUT[] ""*/

/*%typemap(in) std::vector<acv::Target> {
  // TODO
}*/

/*%typemap(in,numinputs=0) std::vector< Target,std::allocator< acv::Target > > (std::vector<acv::Target> tmp_targets) {
  $1 = tmp_targets;
}*/

/*%typemap(out) std::vector<acv::Target> {
  // TODO
}
%typemap(out) std::vector< Target,std::allocator< acv::Target > > = std::vector<acv::Target>;*/

%include stl.i

%include "camera.hpp"
%include "targets.hpp"
