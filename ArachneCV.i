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

using namespace acv;
%}

%typemap(in) double[3] (double temp[3]) {
  int i;
  if (!PySequence_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a sequence");
    return NULL;
  }
  if (PySequence_Length($input) != 3) {
    PyErr_SetString(PyExc_ValueError,"Size mismatch. Expected 4 elements");
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

%include stl.i

%include "camera.hpp"
%include "targets.hpp"
%include "image_analysis.hpp"

%Camera::Camera(int cam_num_in, double cam_coords_in[3], int cam_angle_in, int orientation, int pix_per_ft);
%Camera::GetFrame();
%Camera::GetFrameFromImage(std::string image);
%Camera::WarpPerspective();
%Camera::FindBalls();
%Camera::FindRobots();
%Camera::ShowFrame();
%Camera::~Camera();

%Targets::merge(Target input_targets[16]);
