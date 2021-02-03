//
// Created by loenberg on 2/3/21.
//

#ifndef THESISTESTSOURCE_SORTEDDISTANCEMATRIX_HPP
#define THESISTESTSOURCE_SORTEDDISTANCEMATRIX_HPP

#include "Types.hpp"
#include "Logger.hpp"
#include "Dijkstra.hpp"

#include <vector>

class SortedSubDistanceMatrix{
private:
    vector<int> vertex_indeces;
    vector<vertex_distance> distances;
    bool complete = false;
    int count = 0;
    int size;

    void update(){
        std::sort(distances.begin(), distances.end(), vertex_distance_distance_comparer());
        for (int i = 0; i < size; i++) {
            vertex_indeces[i] = distances[i].v;
        }
    }

public:
    SortedSubDistanceMatrix(int n)
    : distances(n, {-1, INFINITY}),
    vertex_indeces(n){
        for(int i = 0; i < n; i++) vertex_indeces[i] = i;
        size = n;
    }

    vertex_distance getDistance(int v){
        return distances[vertex_indeces[v]];
    }

    void setDistance(int v, double distance){
        if (complete) return;

        bool insert_new = distances[vertex_indeces[v]].distance == INFINITY;
        if (insert_new){
            distances[vertex_indeces[v]].distance = distance;
            count++;
            if (count == size){
                complete = true;
                update();
            }
        }
    }

    void setDistances(vector<vertex_distance> vds){
        for (auto vd: vds){
            setDistance(vd.v,vd.distance);
        }
    }

    void setDistances(vector<double> vds){
        for (int i = 0; i < vds.size(); i++) {
            setDistance(i, vds[i]);
        }
    }

};

class SortedDistanceMatrix{
private:
    int size;
    vector<SortedSubDistanceMatrix> vertex_distances;

public:
    explicit SortedDistanceMatrix(int n)
    : vertex_distances(n, SortedSubDistanceMatrix(n)){
        size = n;
    }

    vertex_distance get_or_update(int u, int v, AdjecencyMatrix &graph, bool undirected = true){
        auto dist = vertex_distances[u].getDistance(v);
        if (dist.distance == INFINITY){
            auto ds = shortest_distances(u, graph, graph.length());
            vertex_distances[u].setDistances(ds);
            if (undirected){
                for (int i = 0; i < size; i++) {
                    vertex_distances[i].setDistance(u, ds[i]);
                }
            }
            return {v, ds[v]};
        }
        else{
            return dist;
        }
    }
};

#endif //THESISTESTSOURCE_SORTEDDISTANCEMATRIX_HPP
