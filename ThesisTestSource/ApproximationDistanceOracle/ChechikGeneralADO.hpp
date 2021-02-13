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
#include "../HelperTypes/ClusterCover.hpp"

#include <unordered_set>
#include <cmath>
#include <tuple>
#include <utility>

double p_i(double x, double d){
    return pow(d, ceil(log(d) * x));
}

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

    unordered_set<int> U;

    DO_U(int k_, unordered_set<int> U_, vector<int> &K_mark, SortedDistanceMatrix &distances, AdjecencyMatrix &graph):
    k(k_),
    p(k, U.size(), [](int i, int j) {return 0;}),
    d(U.size()),
    k_is_even((k % 2) == 0),
    B_i(K_mark.size()),
    U(std::move(U_)){
        prepro(K_mark, distances, graph);
    }

    vector<int> get_uhappy(){
        return {};
    }

private:
    int k;
    Matrix<int> p;
    vector<unordered_map<int, double>> d;

    unordered_map<int,unordered_map<int, vertex_distance>> B_;
    vector<unordered_map<int,unordered_map<int, vertex_distance>>> B_i;
    unordered_map<double, unordered_map<int, cluster>> Home;
    unordered_map<int,unordered_map<int, double>> B_sets_dists;

    const double epsilon = 3./4.;
    bool k_is_even;

    void prepro(vector<int> &K_mark, SortedDistanceMatrix &distances, AdjecencyMatrix &graph){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis (0,1);
        unordered_map<double, unordered_map<int, unordered_set<int>>> W;

        unordered_set<int> A[K_mark.size()];
        for (int i = 0; i < K_mark.size(); i++) {
            sample_A_i(U, i, A[i], K_mark, gen, dis, graph.length());
            for (int v: U) {
                auto closest_node_in_ai = distances.get_lth_closest_node_from_W(v,0,graph, A[i]);
                p(i,v) = closest_node_in_ai.v;
                d[v].insert({closest_node_in_ai.v, closest_node_in_ai.distance});
                auto p_ = p_i(ceil(closest_node_in_ai.distance / (double) K_mark[i]), (1 + epsilon));
                update_W(v, p_, K_mark[i], W);
            }
        }

        for (int v : U){
            B_.insert({v, unordered_map<int, vertex_distance>()});
            for (int u : A[1]){
                auto dist = distances.get_or_update(v, u, graph);
                if (dist.distance < distances.get_or_update(v, p(0, v), graph).distance)
                    B_[v].insert({u, dist});
            }
        }

        // Construct cluster covers
        for (auto [p_val, values]: W){
            Home.insert({p_val, unordered_map<int, cluster>()});
            for (auto [r_val, w] : values){
                Home[p_val].insert({r_val, cluster()});
                build_cluster_cover(w, p_val, r_val, k, U, distances, graph, Home[p_val][r_val]);
            }
        }

        // Calculate distances
        if (k_is_even){
            calculate_distances_between_sets(A[1], A[0], distances, graph);
        }
        else{
            calculate_distances_between_sets(A[0], A[0], distances, graph);
        }

        // B_i
        for (int i = 2; i < K_mark.size(); i++) {
            for (auto v: A[i]){
                B_i[i].insert({v, unordered_map<int, vertex_distance>()});
                for (auto u: A[i]){
                    auto tmp = distances.get_or_update(u, v, graph);
                    if (tmp.distance < distances.get_or_update(v, p(i-1, v), graph).distance){
                        B_i[i][v].insert({u, tmp});
                    }
                }
            }
        }

        // Build DO_short

    }

    void calculate_distances_between_sets(unordered_set<int> s1, unordered_set<int> s2, SortedDistanceMatrix &distances, AdjecencyMatrix &graph){
        for (int u : s1){
            B_sets_dists.insert({u , unordered_map<int, double>()});
            for (int v : s2){
                auto dist = distances.get_or_update(u, v, graph);
                B_sets_dists[u].insert({v, dist.distance});
            }
        }
    }

    void update_W(int v, double p_i, int i, unordered_map<double, unordered_map<int, unordered_set<int>>> &W){
        double p_vals[] = {p_i, p_i/(1+epsilon), (1 + epsilon) * p_i};
        for (double p_ : p_vals){
            if (!W.contains(p_)){
                W.insert({p_, unordered_map<int, unordered_set<int>>()});
                W[p_].insert({i, unordered_set<int>()});
            }
            else{
                if (!W[p_].contains(i)){
                    W[p_].insert({i, unordered_set<int>()});
                }
            }
            W[p_][i].insert(v);
        }
    }

    void sample_A_i(unordered_set<int> &U, int i, unordered_set<int> &A, vector<int> &K_mark, std::mt19937 &gen, std::uniform_real_distribution<double> &dis, int n) const{
        double prob = 1./(pow(n, K_mark[i]/k));
        for (int u : U){
            auto val = dis(gen);
            if(val <= prob){
                A.insert(u);
            }
        }
    }


};


class ChechikGeneralADO: public DistanceOracle{
public:
    ChechikGeneralADO(int k, int v_size, Matrix<double> &distances)
    :ChechikGeneralADO(k, v_size){

    }

    ChechikGeneralADO(int k, int v_size, AdjecencyMatrix &graph)
            :ChechikGeneralADO(k, v_size){
        SortedDistanceMatrix distances(v_size);
        prepro(k, distances, graph);
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

    void prepro(int k, SortedDistanceMatrix &distances, AdjecencyMatrix &graph){
        auto indices = create_indices(k);
        while (!U.empty()){
            auto dou = new DO_U(k, U, indices, distances, graph);
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
