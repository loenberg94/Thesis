//
// Created by loenberg on 1/19/21.
//

#ifndef THESISTESTSOURCE_THORUPSINGLESOURCESHORTESTPATH_HPP
#define THESISTESTSOURCE_THORUPSINGLESOURCESHORTESTPATH_HPP

#include <vector>
#include <unordered_set>
#include "AdjecencyMatrix.hpp"

using namespace std;

struct ShortestPathResult {
    int s, t, w;
    double distance;
    vector<int> path;
};

ShortestPathResult GetShortestPath(AdjecencyMatrix &graph, int s, int t);

#endif //THESISTESTSOURCE_THORUPSINGLESOURCESHORTESTPATH_HPP
