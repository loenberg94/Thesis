cmake_minimum_required(VERSION 3.17)
project(ThesisSource)

set(CMAKE_CXX_STANDARD 20)

add_library(ThesisSource library.cpp library.h ApproximationDistanceOracle/ThorupZwickGeneralADO.hpp BaseClasses/DistanceOracle.hpp HelperTypes/Matrix.hpp)