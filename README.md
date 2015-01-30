#ArachneCV

Written by David Hashe and Jacob Brunson
    
##Introduction

ArachneCV is a Computer Vision library developed by FRC #5212 TAMSformers for use in the FIRST Robotics Competition. It extends OpenCV with a compact and well-documented API suitable for novice and expert programmers alike.

ArachneCV is designed to determine the locations of FRC game objects (bins, noodles, scoring zones, and totes) relative to the robot by using color field detection. It uses the depth-sensing capabilities of OpenNI-enabled cameras where possible to precisely determine position, and falls back to trig approximations of distance for basic cameras. Use of a CUDA-enabled GPU is recommended for best performance, but ArachneCV will fall back to the CPU if necessary.

C++ and Python bindings are available.

##Building ArachneCV

ArachneCV uses CMake, a popular cross-platform build system. As such, it may be built on Linux, Mac, and Windows.

To build ArachneCV, first make sure that OpenCV, SWIG, CMake, and a C++ compiler are present on your system.
IMPORTANT: OpenCV must be built with OpenNI, CUDA, and ffmpeg support. The Ubuntu package has none of these.

Next change into the ArachneCV directory and follow the instructions below.

####Linux & OS X
    
    mkdir build
    cd build
    cmake ..
    make
    make install

####Windows:

Follow instructions for building CMake projects with your IDE.

##Developer Information

ArachneCV is designed with the intent of binding to arbitrary high-level languages to make it equally accessible to FRC teams of all language backgrounds. This is done through SWIG.

ArachneCV documentation is generated through Doxygen.

Implementations of the Camera, DepthCamera, WarpCamera and Target classes live in the src directory. The functions used to search images for objects live in the src/analysis directory. Language bindings live in the src/swig directory.

##Name Explanation

In Greek mythology, Arachne was a talented and proud weaver who rivaled Athena in her abilities. Much like the spider Arachne was transformed into, ArachneCV works by creating and processing a "web" of camera inputs. Through this, it creates a beautiful and functional structure.
