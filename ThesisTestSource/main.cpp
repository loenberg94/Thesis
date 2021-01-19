#include <iostream>
#include <unordered_set>

#include "ApproximationDistanceOracle/ThorupZwickGeneralADO.hpp"
#include "ApproximationDistanceOracle/WulffNilsenGeneralADO.hpp"
#include "HelperTypes/HashTable2Level.hpp"

int main() {

    vector<HashTableEntry> elements = {
            {4, 5.6},
            {7, 3.4},
            {9, 2.8},
            {0, 4},
            {8, 1.9},
            {10, 7},
            {2, 6.7}
    };

    HashTable2Level table(elements, elements.size());

    for (auto elem: elements){
        std::cout << table.contains(elem.key) << std::endl;
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
