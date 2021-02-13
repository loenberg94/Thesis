//
// Created by loenberg on 2/3/21.
//

#ifndef THESISTESTSOURCE_SORTEDDISTANCEMATRIX_HPP
#define THESISTESTSOURCE_SORTEDDISTANCEMATRIX_HPP

#include "Types.hpp"
#include "Logger.hpp"
#include "Dijkstra.hpp"

#include <vector>
#include <unordered_set>

class SortedSubDistanceMatrix{
private:
    vector<int> vertex_indeces;
    vector<vertex_distance> distances;
    int count = 0;
    int size;

    void update(){
        std::sort(distances.begin(), distances.end(), vertex_distance_distance_comparer());
        for (int i = 0; i < size; i++) {
            vertex_indeces[i] = distances[i].v;
        }
    }

public:
    bool complete = false;

    SortedSubDistanceMatrix(int n)
    : distances(n, {-1, INFINITY}),
    vertex_indeces(n){
        for(int i = 0; i < n; i++) vertex_indeces[i] = i;
        size = n;
    }

    vertex_distance getDistance(int v, bool vertex_index = true){
        if (vertex_index)
            return distances[vertex_indeces[v]];
        return distances[v];
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
    bool undirected;
    vector<SortedSubDistanceMatrix> vertex_distances;

    void update_subdistance(int u, AdjecencyMatrix &graph){
        auto ds = shortest_distances(u, graph, graph.length());
        vertex_distances[u].setDistances(ds);
        if (undirected){
            for (int i = 0; i < size; i++) {
                vertex_distances[i].setDistance(u, ds[i]);
            }
        }
    }

public:
    explicit SortedDistanceMatrix(int n, bool undirected = true)
    : vertex_distances(n, SortedSubDistanceMatrix(n)){
        size = n;
        this->undirected = undirected;
    }

    vertex_distance get_or_update(int u, int v, AdjecencyMatrix &graph){
        auto dist = vertex_distances[u].getDistance(v);
        if (dist.distance == INFINITY){
            update_subdistance(u, graph);
            return {v, vertex_distances[u].getDistance(v).distance};
        }
        else{
            return dist;
        }
    }

    vertex_distance get_lth_closest_node(int u, int l, AdjecencyMatrix &graph){
        if (!vertex_distances[u].complete){
            update_subdistance(u, graph);
        }
        return vertex_distances[u].getDistance(l - 1, false);
    }

    vector<vertex_distance> get_lth_closest_nodes(int u, int l, AdjecencyMatrix &graph){
        vector<vertex_distance> l_closest(l);
        if (!vertex_distances[u].complete){
            update_subdistance(u, graph);
        }
        for(int i = 0; i < l; i++){
            l_closest[i] = vertex_distances[u].getDistance(i, false);
        }
        return l_closest;
    }

    vertex_distance get_lth_closest_node_from_W(int u, int l, AdjecencyMatrix &graph, unordered_set<int> &W){
        if (W.size() < l)
            throw "l is bigger than the length of W - get_lth_closest_node_from_W";

        if (!vertex_distances[u].complete){
            update_subdistance(u, graph);
        }

        int count = 0;
        for (int i = 0; i < size; i++) {
            auto v = vertex_distances[u].getDistance(i, false);
            if (W.contains(v.v)){
                count++;
                if (count == l){
                    return v;
                }
            }
        }
        return {-1, INFINITY};
    }

    vector<vertex_distance> get_lth_closest_nodes_from_W(int u, int l, AdjecencyMatrix &graph, unordered_set<int> &W){
        if (W.size() < l)
            throw "l is bigger than the length of W - get_lth_closest_nodes_from_W";

        if (!vertex_distances[u].complete){
            update_subdistance(u, graph);
        }

        int count = 0;

        vector<vertex_distance> l_closest(W.size());
        for (int i = 0; i < size; i++) {
            auto v = vertex_distances[u].getDistance(i, false);
            if (W.contains(v.v)){
                l_closest[count] = v;
                count++;
                if (count == l)
                    break;
            }
        }

        return l_closest;
    }

    vector<vertex_distance> get_nodes_less_than_distance_d(int u, double d, AdjecencyMatrix &graph){
        if (!vertex_distances[u].complete){
            update_subdistance(u, graph);
        }

        int min = 0;
        int max = size - 1;

        while (min <= max) {
            int m = min + (max - min) / 2;

            if (vertex_distances[u].getDistance(m, false).distance <= d)
                min = m + 1;
            else
                max = m - 1;
        }

        vector<vertex_distance> nodes_less_than(max + 1);
        for (int i = 0; i <= max; i++) {
            nodes_less_than[i] = vertex_distances[u].getDistance(i, false);
        }

        return nodes_less_than;
    }


};

#endif //THESISTESTSOURCE_SORTEDDISTANCEMATRIX_HPP
