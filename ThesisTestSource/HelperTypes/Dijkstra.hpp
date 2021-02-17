//
// Created by loenberg on 2/2/21.
//

#ifndef THESISTESTSOURCE_DIJKSTRA_HPP
#define THESISTESTSOURCE_DIJKSTRA_HPP

#include <unordered_set>

#include "../HelperTypes/Types.hpp"
#include "../HelperTypes/AdjecencyMatrix.hpp"
#include "MinHeap.hpp"
#include "Matrix.hpp"

double shortest_distance(int s, int t, AdjecencyMatrix &graph, int v_size);
vector<double> shortest_distances(int s, AdjecencyMatrix &graph, int v_size);


class Dijkstra{
private:
    vector<int> prev;
    vector<double> d;
    vector<bool> visited;
    int size;

    void reset(){
        for (int i = 0; i < size; i++) {
            prev[i] = -1;
            d[i] = INFINITY;
            visited[i] = false;
        }
    }

public:
    explicit Dijkstra(int v_size):
    prev(v_size, -1),
    d(v_size, INFINITY),
    size(v_size),
    visited(v_size, false){

    }

    vector<int>& get_prev(){
        return prev;
    }

    vector<double>& get_d(){
        return d;
    }

    void shortest_distances_with_w(int s, AdjecencyMatrix &graph, unordered_set<int> &A_i){
        MinHeap dpq (size);
        dpq.insertKey(s, 0);
        for (int j = 0; j < size; j++) {
            if (j != s)
                dpq.insertKey(j, INFINITY);
        }

        reset();
        d[s] = 0;

        QueueItem q = dpq.extractMin();
        while (q.v != -1){
            if (A_i.contains(q.v))
                prev[q.v] = q.v;
            unordered_map<int,Entry>& adjecent_vertices = graph[q.v];
            for (auto [key, value]: adjecent_vertices){
                if (visited[key])
                    continue;
                double tmp_dist = d[q.v] + value.weight;
                if (tmp_dist < d[key]){
                    if (!A_i.contains(key))
                        prev[key] = prev[q.v];
                    d[key] = tmp_dist;
                    dpq.decreaseKey(key, d[key]);
                }
            }
            visited[q.v] = true;
            q = dpq.extractMin();
        }
    }

};


#endif //THESISTESTSOURCE_DIJKSTRA_HPP
