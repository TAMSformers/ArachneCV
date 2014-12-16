#
# Arachne CV
#
# Written by David Hashe and Jacob Brunson
#
#
#

if(OUTPUT_PYTHON)

configure_file(
  "${PROJECT_SOURCE_DIR}/tests/python/camera_warp.py"
  "${PROJECT_BINARY_DIR}/tests/python/camera_warp.py"
)
configure_file(
  "${PROJECT_SOURCE_DIR}/tests/python/image_warp.py"
  "${PROJECT_BINARY_DIR}/tests/python/image_warp.py"
)
configure_file(
  "${PROJECT_SOURCE_DIR}/tests/python/video_warp.py"
  "${PROJECT_BINARY_DIR}/tests/python/video_warp.py"
)

endif(OUTPUT_PYTHON)
