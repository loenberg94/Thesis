//
// Created by loenberg on 2/8/21.
//

#include "ClusterCover.hpp"
#include "Types.hpp"
#include "SortedDistanceMatrix.hpp"
#include "AdjecencyMatrix.hpp"

#include <unordered_set>

using namespace std;

struct vertices_bunch{
    int i;
    vector<vertex_distance> intersection;
};

vertex_distance pick_v(unordered_set<int> &V_prime, unordered_set<int> &U, int r, int k, SortedDistanceMatrix &distances, AdjecencyMatrix &graph);
vertices_bunch find_i(int v, double d, int k, unordered_set<int> &V_prime, SortedDistanceMatrix &distances, AdjecencyMatrix &graph);
vector<vertex_distance> update_home(int v, int i, double d, unordered_set<int> &V_prime, SortedDistanceMatrix &distances, AdjecencyMatrix &graph, cluster &c, vector<vertex_distance> &cover);
void update_V_prime(vector<vertex_distance> &b_i_minus_one, unordered_set<int> &V_prime);
vector<vertex_distance> intersection_(vector<vertex_distance> &b, unordered_set<int> &V_prime);

void build_cluster_cover(unordered_set<int> &V_prime, double p_, int r, int k, unordered_set<int> &U, SortedDistanceMatrix &distances, AdjecencyMatrix &graph, cluster &c){
    while (!V_prime.empty()){
        auto v = pick_v(V_prime, U, r, k, distances, graph);
        double d = min(v.distance/(double) r, p_);
        vertices_bunch index = find_i(v.v, d, k, V_prime, distances, graph);
        auto b_i_minus_one = update_home(v.v, index.i, d, V_prime, distances, graph, c, index.intersection);
        update_V_prime(b_i_minus_one, V_prime);
    }
}

vertex_distance pick_v(unordered_set<int> &V_prime, unordered_set<int> &U, int r, int k, SortedDistanceMatrix &distances, AdjecencyMatrix &graph){
    double max_dist = -1;
    int max_node = -1;
    for (int i : V_prime){
        auto d = D(i, U, pow(graph.length(), r/(double)k),distances, graph);
        if (d.distance > max_dist){
            max_dist = d.distance;
            max_node = i;
        }
    }
    return {max_node, max_dist};
}

vertices_bunch find_i(int v, double d, int k, unordered_set<int> &V_prime, SortedDistanceMatrix &distances, AdjecencyMatrix &graph){
    int i = 1;

    while(true){
        auto b = B(v, i * d, distances, graph);
        auto intersection = intersection_(b, V_prime);
        if (intersection.size() <= pow(graph.length(), i/(double) k)){
            return {i, intersection};
        }
    }
}

vector<vertex_distance> update_home(int v, int i, double d, unordered_set<int> &V_prime, SortedDistanceMatrix &distances, AdjecencyMatrix &graph, cluster &c, vector<vertex_distance> &cover){
    auto b_i_minus_one = B(v, (i - 1) * d, distances, graph);
    for (auto u: b_i_minus_one){
        c.insert({u.v, &cover});
    }
    return b_i_minus_one;
}

void update_V_prime(vector<vertex_distance> &b_i_minus_one, unordered_set<int> &V_prime){
    for (auto vd : b_i_minus_one)
        V_prime.erase(vd.v);
}

vector<vertex_distance> intersection_(vector<vertex_distance> &b, unordered_set<int> &V_prime){
    vector<vertex_distance> inter;
    for (auto vd : b){
        if (V_prime.contains(vd.v))
            inter.push_back(vd);
    }
    return inter;
}

