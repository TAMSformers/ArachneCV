#
# Arachne CV
#
# Written by David Hashe and Jacob Brunson
#
#
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
)
set_target_properties(ArachneCV_CSharp PROPERTIES OUTPUT_NAME ArachneCV)

swig_link_libraries(ArachneCV_CSharp ${OpenCV_LIBRARIES})

