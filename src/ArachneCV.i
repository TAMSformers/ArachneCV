/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 */

%module ArachneCV

%{
#include "camera.hpp"
#include "targets.hpp"
#include "image_analysis.hpp"

/* SWIG doesn't recognize Target as part of acv, for some reason. */
using acv::Target;
%}

%typemap(in) double[3] (double temp[3]) {
  int i;
  if (!PySequence_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a sequence");
    return NULL;
  }
  if (PySequence_Length($input) != 3) {
    PyErr_SetString(PyExc_ValueError,"Size mismatch. Expected 3 elements");
    return NULL;
  }
  for (i = 0; i < 3; i++) {
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
    std::string color;
    PyObject *py_color = PySequence_GetItem(PySequence_GetItem($input,i),1);
    if (PyString_Check(py_color)) {
      color = PyString_AsString(py_color);
    } else {
      PyErr_SetString(PyExc_ValueError,"color is not a string");
      return NULL;
    }
    double coords[3];
    for (int j = 0; j < 3; j++) {
      PyObject *o = PySequence_GetItem(PySequence_GetItem(PySequence_GetItem($input,i),2),j);
      if (PyNumber_Check(o)) {
        coords[j] = (double) PyFloat_AsDouble(o);
      } else {
        PyErr_SetString(PyExc_ValueError,"coords elements must be numbers");
        return NULL;
      }
    }
    acv::Target tmp_target;
    tmp_targets[i].type = type;
    tmp_targets[i].color = color;
    tmp_targets[i].coords[0] = coords[0];
    tmp_targets[i].coords[1] = coords[1];
    tmp_targets[i].coords[2] = coords[2];
    tmp_targets[i].is_real = true;
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
    PyList_SetItem(py_target,1,PyString_FromString((&$1)[0][i].color.c_str()));
    PyList_SetItem(py_target,2,PyList_New(3));
    for (int j = 0; j < 3; j++) {
      PyObject *o = PyFloat_FromDouble((double)((&$1)[0][i].coords[j]));
      PyList_SetItem(PySequence_GetItem(py_target,2),j,o);
    }
    PyList_SetItem(py_target,3,PyList_New(0));
    PyList_Append($result,py_target);
  }
}
%typemap(out) std::vector< Target,std::allocator< acv::Target > > = std::vector<acv::Target>;

%include stl.i

%include "camera.hpp"
%include "targets.hpp"
%include "image_analysis.hpp"
