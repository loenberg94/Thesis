//
// Created by loenberg on 2/2/21.
//

#ifndef THESISTESTSOURCE_DIJKSTRA_HPP
#define THESISTESTSOURCE_DIJKSTRA_HPP

#include "../HelperTypes/Types.hpp"
#include "../HelperTypes/AdjecencyMatrix.hpp"

double shortest_distance(int s, int t, AdjecencyMatrix &graph, int v_size);
vector<double> shortest_distances(int s, AdjecencyMatrix &graph, int v_size);
index_value shortest_distances_with_w(int s, int t, AdjecencyMatrix &graph, int v_size);


#endif //THESISTESTSOURCE_DIJKSTRA_HPP
