//
// Created by loenberg on 12/21/20.
//

#ifndef THESISTESTSOURCE_CHECHIKGENERALADO_HPP
#define THESISTESTSOURCE_CHECHIKGENERALADO_HPP

#include "../BaseClasses/DistanceOracle.hpp"
#include "../HelperTypes/Types.hpp"
#include "../HelperTypes/Dijkstra.hpp"
#include "../HelperTypes/DistanceMatrix.hpp"
#include "../HelperTypes/SortedDistanceMatrix.hpp"

#include <unordered_set>
#include <cmath>
#include <tuple>

// NN(u, W, l) -> "l"th nearest nodes to u from W
// M(u, W, l) -> "l"th closest node to u from W
// D(u, W, l) -> dist(u, M(u, W, l))
// B(v, d) -> all nodes at distance strictly less than d from v, v = node, d = distance


vector<vertex_distance> NN(int u, unordered_set<int> &W, int l, DistanceMatrix &distances, AdjecencyMatrix &graph){
    vector<vertex_distance> l_closest_nodes(l);
    vector<vertex_distance> w_distances(W.size(), {-1, INFINITY});
    int i = 0;
    for (int v : W){
        w_distances[i] = distances.get_or_update(u, v, graph);
        i++;
    }
    std::sort(w_distances.begin(), w_distances.end(), vertex_distance_distance_comparer());

    for (int j = 0; j < l; j++) {
        l_closest_nodes[j] = w_distances[j];
    }
    return l_closest_nodes;
}

vertex_distance M(int u, unordered_set<int> &W, int l, DistanceMatrix &distances, AdjecencyMatrix &graph){
    vector<vertex_distance> w_distances(W.size(), {-1, INFINITY});
    int i = 0;
    for (int v : W){
        w_distances[i] = distances.get_or_update(u, v, graph);
        i++;
    }
    std::sort(w_distances.begin(), w_distances.end(), vertex_distance_distance_comparer());
    return w_distances[l -1];
}

vertex_distance D(int u, unordered_set<int> &W, int l, DistanceMatrix &distances, AdjecencyMatrix &graph){
    return distances.get_or_update(u, M(u, W, l,distances, graph).v, graph);
}

vector<vertex_distance> B(int v, double d, DistanceMatrix &distances, AdjecencyMatrix &graph){
    vector<vertex_distance> w_distances(graph.length(), {-1, INFINITY});
    int i = 0;
    /**for (int v : W){
        w_distances[i] = distances.get_or_update(u, v, graph);
        i++;
    }**/
}






class ClusterCover{


public:
    ClusterCover(unordered_set<int> W, double p_, int r, int k){

        prepro(W, p_, r);
    }

private:


    void prepro(unordered_set<int> &W, double p_, int r){
        while (!W.empty()){

        }
    }

};

class DO_short{

};

class DO_U{
    //1. Pivots
    //2. Bunches B[(k/2)-1](v)
    //3. CC - Cluster Covers
    //4. Distances between A[(k/2) - 1] and A[k/2] or A[k/2] and A[k/2]
    //5. B_i(v) for every i in K\{k/2 - 1, k/2} and v in A_i
    //6. DO_short

public:
    vector<int> get_uhappy(){
        return {};
    }

private:

};


class ChechikGeneralADO: public DistanceOracle{
public:
    ChechikGeneralADO(int k, int v_size, Matrix<double> &distances)
    :ChechikGeneralADO(k, v_size){

    }

    double GetDistance(int u, int v) override {
        return 0;
    }

    int GetSize() override {
        return size;
    }

protected:
    int mK;

private:
    int size = 0;
    unordered_set<int> U;
    vector<DO_U*> dous;

    ChechikGeneralADO(int k, int v_size):
        U(v_size),
        dous(v_size){
        mK = k;
        for (int i = 0; i < v_size; i++)
            U.insert(i);
    }

    void prepro(int k){
        auto indices = create_indices(k);
        while (!U.empty()){
            auto dou = new DO_U();
            for (int u : dou->get_uhappy()) {
                U.erase(u);
                dous[u] = dou;
            }


        }
    }

    vector<int> create_indices(int k){
        vector<int> ks = {k/2, (k/2) - 1, (int) ceil(k/4.), (int) ceil(k/8.),
                     (int) ceil(k/16.), (int) ceil(k/32.), (int) ceil(k/64.),
                     (int) ceil(k/128.), (int) ceil(k/256.)};

        vector<int> indeces;
        unordered_set<int> seen;

        for (auto _k: ks){
            if (!seen.contains(_k)){
                seen.insert(_k);
                indeces.push_back(_k);
            }
        }

        return indeces;
    }

};

#endif //THESISTESTSOURCE_CHECHIKGENERALADO_HPP
