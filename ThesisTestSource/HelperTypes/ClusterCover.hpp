//
// Created by loenberg on 2/8/21.
//

#ifndef THESISTESTSOURCE_CLUSTERCOVER_HPP
#define THESISTESTSOURCE_CLUSTERCOVER_HPP

#include "Types.hpp"
#include "SortedDistanceMatrix.hpp"
#include "AdjecencyMatrix.hpp"

#include <vector>
#include <unordered_set>

#define cluster std::unordered_map<int, vector<vertex_distance>*>

using namespace std;

vector<vertex_distance> NN(int u, unordered_set<int> &W, int l, SortedDistanceMatrix &distances, AdjecencyMatrix &graph){
    return distances.get_lth_closest_nodes_from_W(u, l, graph, W);
}

vertex_distance M(int u, unordered_set<int> &W, int l, SortedDistanceMatrix &distances, AdjecencyMatrix &graph){
    return distances.get_lth_closest_node_from_W(u, l, graph, W);
}

vertex_distance D(int u, unordered_set<int> &W, int l, SortedDistanceMatrix &distances, AdjecencyMatrix &graph){
    return distances.get_or_update(u, M(u, W, l,distances, graph).v, graph);
}

vector<vertex_distance> B(int v, double d, SortedDistanceMatrix &distances, AdjecencyMatrix &graph){
    return distances.get_nodes_less_than_distance_d(v, d, graph);
}


void build_cluster_cover(unordered_set<int> &V_prime, double p_, int r, int k, unordered_set<int> &U, SortedDistanceMatrix &distances, AdjecencyMatrix &graph, cluster &c);

#endif //THESISTESTSOURCE_CLUSTERCOVER_HPP
