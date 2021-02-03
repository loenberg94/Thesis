#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>

#include "ApproximationDistanceOracle/ThorupZwickGeneralADO.hpp"
#include "ApproximationDistanceOracle/WulffNilsenGeneralADO.hpp"
#include "ApproximationDistanceOracle/ChechikGeneralADO.hpp"
#include "HelperTypes/HashTable2Level.hpp"
#include "HelperTypes/Dijkstra.hpp"

using namespace std;


int main() {

    Matrix<double> t(2);
    t(1,1) = 4;

    std::cout << t(1,1) << std::endl;
    std::cout << t(0,0) << std::endl;
    std::cout << t(1,0) << std::endl;
    std::cout << t(0,1) << std::endl;
    return 0;
}
