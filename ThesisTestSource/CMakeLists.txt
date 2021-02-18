cmake_minimum_required(VERSION 3.17)
project(ThesisTestSource)

FIND_PACKAGE(OpenMP REQUIRED)
if(OPENMP_FOUND)
    # set(CMAKE_C_FLAGS "${OpenMP_C_FLAGS}")
    # set(CMAKE_CXX_FLAGS "${OpenMP_C_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 20)

add_executable(ThesisTestSource main.cpp BaseClasses/DistanceOracle.hpp ApproximationDistanceOracle/ThorupZwickGeneralADO.hpp HelperTypes/Matrix.hpp ApproximationDistanceOracle/ChechikGeneralADO.hpp ApproximationDistanceOracle/WulffNilsenGeneralADO.hpp HelperTypes/Edge.hpp HelperTypes/HashTable2Level.hpp HelperTypes/PrimeGenerator.hpp HelperTypes/PrimeGenerator.cpp HelperTypes/AdjecencyMatrix.hpp HelperTypes/ThorupSingleSourceShortestPath.hpp HelperTypes/ThorupSingleSourceShortestPath.cpp HelperTypes/MinHeap.hpp HelperTypes/Dijkstra.cpp HelperTypes/Dijkstra.hpp HelperTypes/Types.hpp HelperTypes/DistanceMatrix.hpp HelperTypes/SortedDistanceMatrix.hpp HelperTypes/Logger.hpp HelperTypes/Logger.cpp HelperTypes/ClusterCover.hpp HelperTypes/ClusterCover.cpp)