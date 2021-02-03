//
// Created by loenberg on 2/2/21.
//

#include "Dijkstra.hpp"
#include "MinHeap.hpp"
#include <unordered_set>

vector<double> shortest_distances(int s, AdjecencyMatrix &graph, int v_size){
    MinHeap dpq (v_size);
    dpq.insertKey(s, 0);
    for (int j = 0; j < v_size; j++) {
        if (j != s)
            dpq.insertKey(j, INFINITY);
    }

    vector<double> d_temp(v_size, INFINITY);
    d_temp[s] = 0;
    unordered_set<int> visited_vertices;

    QueueItem q = dpq.extractMin();
    while (q.v != -1){
        if (!visited_vertices.contains(q.v)){
            auto adjecent_vertices = graph[q.v];
            for (auto [key, value]: adjecent_vertices){
                double tmp_dist = d_temp[q.v] + value.weight;
                if (tmp_dist < d_temp[key]){
                    d_temp[key] = tmp_dist;
                    dpq.decreaseKey(key, d_temp[key]);
                }
            }
            visited_vertices.insert(q.v);
        }
        q = dpq.extractMin();
    }

    return d_temp;
}

double shortest_distance(int s, int t, AdjecencyMatrix &graph, int v_size){
    return shortest_distances(s, graph, v_size)[t];
}


