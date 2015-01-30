#ArachneCV

Written by David Hashe and Jacob Brunson
    
##Introduction

ArachneCV is a Computer Vision library developed by FRC #5212 TAMSformers for use in the FIRST Robotics Competition. It extends OpenCV with a compact and well-documented API suitable for novice and expert programmers alike.

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

##Name Explanation

In Greek mythology, Arachne was a talented and proud weaver who rivaled Athena in her abilities. Much like the spider Arachne was transformed into, ArachneCV works by creating and processing a "web" of camera inputs. Through this, it creates a beautiful and functional structure.
