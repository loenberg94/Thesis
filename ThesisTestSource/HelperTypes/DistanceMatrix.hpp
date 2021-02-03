//
// Created by loenberg on 2/2/21.
//

#ifndef THESISTESTSOURCE_DISTANCEMATRIX_HPP
#define THESISTESTSOURCE_DISTANCEMATRIX_HPP

#include "Matrix.hpp"
#include "Dijkstra.hpp"

class DistanceMatrix: public Matrix<double>{

public:
    DistanceMatrix(int n, double (*constructor)(int,int))
    :Matrix<double>(n, constructor){
    }

    explicit DistanceMatrix(int n)
    :Matrix<double>(n, [](int i, int j) {return -1.;}){
    }

    vertex_distance get_or_update(int u, int v, AdjecencyMatrix &graph, bool undirected = true){
        if (this->operator()(u, v) == -1){
            auto dist = shortest_distances(u, graph, graph.length());
            for (int i = 0; i < dist.size(); i++) {
                this->operator()(u,i) = dist[i];
                if (undirected) this->operator()(i, u) = dist[i];
            }
            return {v, dist[v]};
        }
        else{
            return {v, this->operator()(u,v)};
        }
    }

};

#endif //THESISTESTSOURCE_DISTANCEMATRIX_HPP
