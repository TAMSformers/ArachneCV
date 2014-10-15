#ArachneCV

Written by David Hashe and Jacob Brunson
    
##Introduction

ArachneCV is a Computer Vision library developed by FRC #5212 TAMSformers for use in the FIRST Robotics Competition. It extends OpenCV with a compact and well-documented API suitable for novice and expert programmers alike.

Bindings are available to all languages supported by SWIG.

##Building ArachneCV

ArachneCV uses CMake, a popular cross-platform build system. As such, it may be built on Linux, Mac, and Windows.

To build ArachneCV, first make sure that OpenCV, SWIG, CMake, and a C++ compiler are present on your system.

Next change into the ArachneCV directory and follow the instructions below.

####Linux & OS X
    
    mkdir build
    cd build
    cmake ..
    make
    make install

####Windows:

*TODO* Idk how Windows works.

##Developer Information

ArachneCV is designed with the intent of binding to arbitrary high-level languages to make it equally accessible to FRC teams of all language backgrounds. This is done through SWIG. However, because SWIG only supports the C++ Standard Library and not OpenCV, proxy classes have been created to mediate between SWIG and OpenCV. These classes are specified in ArachneCV.cpp and ArachneCV.hpp.

Furthermore, all internal functions and classes are prefixed "acv".

ArachneCV documentation is generated through Doxygen.

##Name Explanation

In Greek mythology, Arachne was a talented and proud weaver who rivaled Athena in her abilities. Much like the spider Arachne was transformed into, ArachneCV works by creating and processing a "web" of camera inputs. Through this, it creates a beautiful and functional structure.
