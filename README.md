This repository contains code and classes that can extract/deserialise ADCP data formats from Origin products
in a selection of supported languages.

AGRAMs / BGRAMs Sonardyne formats are supported
PD0 (with Sonardyne extensions) are also supported

====

/src  contains the deserialisation classes (in DGRAMS - AGRAM and BGRAM deserialiser, PDN = PD0 deserialiser)

/examples contain deserialisation examples for C++, python and octave/matlab

#To build/run the C++ examples

Dependencies: CMake >= 3.15, g++, make, build-essential, gcc

go to /examples/cpp/
run build_all.sh

in each of the examples build/bin/ folder the executable can be invoked on a test file (*.agram/*.bgram/*.pd0)

GramFileDemonstrator file.bgram
GramFileDemonstrator file.agram


The stream examples can be hooked to the adcp portal on the port outputting a stream of relevant data:

For (e.g) BGRAMS (if output on port 4011 - check portal for port number):

GramStreamDemonstrator bgram 192.168.179.20 4011


#To run the Python 3 examples

Dependencies: Python 3

Similar operation to the C++ examples


