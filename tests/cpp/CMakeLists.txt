#
# Arachne CV
#
# Written by David Hashe and Jacob Brunson
#
#
#

if(OUTPUT_NATIVE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_custom_target(tests)

add_dependencies(tests ArachneCV)

add_executable(camera_warp camera_warp.cpp)
add_executable(image_warp image_warp.cpp)
add_executable(multiple_warp multiple_warp.cpp)
add_executable(image_depth image_depth.cpp)

add_dependencies(camera_warp tests)
add_dependencies(image_warp tests)
add_dependencies(multiple_warp tests)
add_dependencies(image_depth tests)

find_package(OpenCV REQUIRED)

target_link_libraries(camera_warp ArachneCV ${OpenCV_LIBS} ${freenect2_LIBS})
target_link_libraries(image_warp ArachneCV ${OpenCV_LIBS} ${freenect2_LIBS})
target_link_libraries(multiple_warp ArachneCV ${OpenCV_LIBS} ${freenect2_LIBS})
target_link_libraries(image_depth ArachneCV ${OpenCV_LIBS} ${freenect2_LIBS})

endif(OUTPUT_NATIVE)
