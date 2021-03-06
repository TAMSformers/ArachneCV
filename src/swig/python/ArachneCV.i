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

%typemap(in) double[3] (double temp[3]) {
  if (!PySequence_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a sequence");
    return NULL;
  }
  if (PySequence_Length($input) != 3) {
    PyErr_SetString(PyExc_ValueError,"Size mismatch. Expected 3 elements");
    return NULL;
  }
  for (int i = 0; i < 3; i++) {
    PyObject *o = PySequence_GetItem($input,i);
    if (PyNumber_Check(o)) {
      temp[i] = (double) PyFloat_AsDouble(o);
    } else {
      PyErr_SetString(PyExc_ValueError,"Sequence elements must be numbers");
      return NULL;
    }
  }
  $1 = temp;
}

%typemap(in) std::vector<cv::Point> (std::vector<cv::Point> points) {
  if (!PySequence_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a sequence");
    return NULL;
  }
  int num_points = PySequence_Length($input);
  for (int i = 0; i < num_points; i++) {
    if (!PySequence_Check(PySequence_GetItem($input,i))) {
      PyErr_SetString(PyExc_ValueError,"Each element must be a sequence");
      return NULL;
    }
    if (PySequence_Length(PySequence_GetItem($input,i)) != 2) {
      PyErr_SetString(PyExc_ValueError,"Each element must be of length 2");
      return NULL;
    }
    int x = PyInt_AsLong(PySequence_GetItem(PySequence_GetItem($input,i),0));
    int y = PyInt_AsLong(PySequence_GetItem(PySequence_GetItem($input,i),1));
    cv::Point p(x,y);
    points.push_back(p);
  }
  $1 = points;
}
/*
%typemap(out) double[3] {
  $result = PyList_New(3);
  for (i = 0; i < 3; i++) {
    PyObject *o = PyFloat_FromDouble((double)$1[i]);
    PyList_SetItem($result,i,o);
  }
}
*/

%typemap(in) std::vector<acv::Target> {
  std::vector<acv::Target> tmp_targets(PySequence_Length($input));
  if (!PySequence_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a sequence");
    return NULL;
  }
  for (int i = 0; i < PySequence_Length($input); i++) {
    if (!PySequence_Check($input)) {
      PyErr_SetString(PyExc_ValueError,"Expected a sequence of sequences");
      return NULL;
    }
    if (PySequence_Length(PySequence_GetItem($input,i)) != 4) {
      PyErr_SetString(PyExc_ValueError,"Size mismatch. Expected 4 elements");
      return NULL;
    }
    std::string type;
    PyObject *py_type = PySequence_GetItem(PySequence_GetItem($input,i),0);
    if (PyString_Check(py_type)) {
      type = PyString_AsString(py_type);
    } else {
      PyErr_SetString(PyExc_ValueError,"type is not a string");
      return NULL;
    }
    double coords[3];
    for (int j = 0; j < 3; j++) {
      PyObject *o = PySequence_GetItem(PySequence_GetItem(PySequence_GetItem($input,i),1),j);
      if (PyNumber_Check(o)) {
        coords[j] = (double) PyFloat_AsDouble(o);
      } else {
        PyErr_SetString(PyExc_ValueError,"coords elements must be numbers");
        return NULL;
      }
    }
    double angle;
    PyObject *py_angle = PySequence_GetItem(PySequence_GetItem($input,i),2);
    if (PyNumber_Check(py_angle)) {
      angle = PyFloat_AsDouble(py_angle);
    } else {
      PyErr_SetString(PyExc_ValueError,"angle is not a number");
      return NULL;
    }
    std::string orientation;
    PyObject *py_orientation = PySequence_GetItem(PySequence_GetItem($input,i),3);
    if (PyString_Check(py_orientation)) {
      orientation = PyString_AsString(py_orientation);
    } else {
      PyErr_SetString(PyExc_ValueError,"orientation is not a string");
      return NULL;
    }
    acv::Target tmp_target;
    tmp_targets[i].type = type;
    tmp_targets[i].coords[0] = coords[0];
    tmp_targets[i].coords[1] = coords[1];
    tmp_targets[i].coords[2] = coords[2];
    tmp_targets[i].angle = angle;
    tmp_targets[i].orientation = orientation;
  }
  $1 = tmp_targets;
}

/*%typemap(in,numinputs=0) std::vector< Target,std::allocator< acv::Target > > (std::vector<acv::Target> tmp_targets) {
  $1 = tmp_targets;
}*/

%typemap(out) std::vector<acv::Target> {
  $result = PyList_New(0);
  for (int i = 0; i < $1.size(); i++) {
    PyObject *py_target = PyList_New(4);
    PyList_SetItem(py_target,0,PyString_FromString((&$1)[0][i].type.c_str()));
    PyObject *py_pos = PyList_New(3);
    for (int j = 0; j < 3; j++) {
      PyObject *o = PyFloat_FromDouble((double)((&$1)[0][i].coords[j]));
      PyList_SetItem(py_pos,j,o);
    }
    PyList_SetItem(py_target,1,PyList_AsTuple(py_pos));
    PyList_SetItem(py_target,2,PyFloat_FromDouble((&$1)[0][i].angle));
    PyList_SetItem(py_target,3,PyString_FromString((&$1)[0][i].orientation.c_str()));
    PyList_Append($result,PyList_AsTuple(py_target));
  }
}
%typemap(out) std::vector< Target,std::allocator< acv::Target > > = std::vector<acv::Target>;

%include stl.i

%include "camera.hpp"
%include "targets.hpp"
