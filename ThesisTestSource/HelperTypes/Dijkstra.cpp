//
// Created by loenberg on 2/2/21.
//

#include "Dijkstra.hpp"
#include "MinHeap.hpp"
#include "Types.hpp"
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
    vector<bool> visited_vertices(v_size);

    QueueItem q = dpq.extractMin();
    while (q.v != -1){
        if (!visited_vertices[q.v]){
            auto adjecent_vertices = graph[q.v];
            for (auto edge: adjecent_vertices){
                double tmp_dist = d_temp[q.v] + edge.weight;
                if (tmp_dist < d_temp[edge.v]){
                    d_temp[edge.v] = tmp_dist;
                    dpq.decreaseKey(edge.v, d_temp[edge.v]);
                }
            }
            visited_vertices[q.v] = true;
        }
        q = dpq.extractMin();
    }

    return d_temp;
}

double shortest_distance(int s, int t, AdjecencyMatrix &graph, int v_size){
    return shortest_distances(s, graph, v_size)[t];
}




