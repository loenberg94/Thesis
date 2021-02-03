//
// Created by loenberg on 12/19/20.
//



#ifndef THESISSOURCE_THORUPZWICKGENERALADO_HPP
#define THESISSOURCE_THORUPZWICKGENERALADO_HPP

#include "../BaseClasses/DistanceOracle.hpp"
#include "../HelperTypes/Matrix.hpp"
#include "../HelperTypes/Edge.hpp"
#include "../HelperTypes/HashTable2Level.hpp"
#include "../HelperTypes/AdjecencyMatrix.hpp"
#include "../HelperTypes/ThorupSingleSourceShortestPath.hpp"
#include "../HelperTypes/MinHeap.hpp"
#include <cassert>
#include <memory>
#include <set>
#include <iterator>
#include <cmath>
#include <random>
#include <tuple>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>

using namespace std;

class ThorupZwickGeneralADO: public DistanceOracle {
public:
    ThorupZwickGeneralADO(int k, int v_size, Matrix<double> &distances)
    :ThorupZwickGeneralADO(k, v_size){
        prepro_metric(k, v_size, distances);
    }

    ThorupZwickGeneralADO(int k, int v_size, vector<Edge> &edges)
    :ThorupZwickGeneralADO(k, v_size){
        prepro_graph(k, v_size, edges);
    }

    double GetDistance(int u, int v) override{
        int w = u; int i = 0;
        while (!bunches[v]->contains(w)){
            i++;
            int tmp = v;
            v = u;
            u = tmp;
            w = p(i, u);
        }
        return d(i, u) + bunches[v]->operator[](w);
    }

    int GetSize() override {
        return size;
    }

protected:
    vector<unique_ptr<HashTable2Level>> bunches;
    Matrix<int> p;
    Matrix<double> d;
    int k_;

private:
    int size = 0;

    ThorupZwickGeneralADO(int k, int v_size){
        p = Matrix<int> (k, v_size, [](int i, int j) {return 0;});
        d = Matrix<double> (k + 1, v_size, [](int i, int j) {return 0.0;});
        k_ = k;
    }

    void prepro_metric(int k, int v_size, Matrix<double> distances){
        std::random_device rd;
        std::mt19937 gen (rd());
        std::uniform_real_distribution<double> dis (0,1);

        std::list<int> a_differences[k];
        std::unordered_set<int> A[k + 1];

        for (int i = 0; i < v_size; i++) {
            A[0].insert(i);
        }

        for (int i = 1; i < k; i++) {
            auto s = A[i - 1];
            for (int elem: s){
                auto prob = dis(gen);
                if (prob <= std::pow((double)v_size, -(1.0/(double)k))){
                    A[i].insert(elem);
                }
                else{
                    a_differences[i - 1].push_back(elem);
                }
            }
        }

        for (int v = 0; v < v_size; v++){
            for (int i = 0; i < k; i++) {
                const auto [pi, dist] = min_dist(v, A[i], distances);
                p(i, v) = pi;
                d(i, v) = dist;
            }
            d(k, v) = INFINITY;
        }

        for (int v = 0; v < v_size; v++){
            for (int i = 0; i < k; i++) {
                for (int w: a_differences[i]){
                    double tmp_dist = distances(w, v);
                    if(tmp_dist < d(i + 1, v)){
                        //bunches[v].insert({w, tmp_dist});
                        size++;
                    }
                }
            }
        }
    }

    void prepro_graph(int k, int v_size, vector<Edge> edges){
        AdjecencyMatrix adj(v_size, edges);
        std::random_device rd;
        std::mt19937 gen (rd());
        std::uniform_real_distribution<double> dis (0,1);
        std::list<int> a_differences[k];
        std::unordered_set<int> A[k + 1];

        for (int i = 0; i < v_size; i++) {
            A[0].insert(i + 1);
            d(k, i) = INFINITY;
        }

        for (int i = 1; i < k; i++) {
            auto s = A[i - 1];
            for (int elem: s){
                auto prob = dis(gen);
                if (prob <= std::pow((double)v_size, -(1.0/(double)k))){
                    A[i].insert(elem);
                }
                else{
                    a_differences[i - 1].push_back(elem);
                }
            }
        }

        vector<unordered_map<int, double>> C(v_size);

        for (int i = k - 1; i < 0; i--) {
            for (int v = 0; v < v_size; v++) {
                // Add extra column to adj matrix, symb. s
                auto dAi = GetShortestPath(adj, v_size, v);
                p(i, v) = dAi.w;
                d(i, v) = dAi.distance;
            }

            adj.remove_last_row();

            for (int w: a_differences[i]){
                grow_shortest_dist_tree(adj, C, w, i, v_size);
            }
        }

        for (int v = 0; v < v_size; v++) {
            vector<HashTableEntry> temp_bunches;
            for (int w = 0; w < v_size; w++) {
                if (C[w].contains(v))
                    temp_bunches.push_back({w, C[w][v]});
            }
            bunches.push_back(std::make_unique<HashTable2Level>(temp_bunches, temp_bunches.size()));
        }
    }

    void grow_shortest_dist_tree(AdjecencyMatrix graph, vector<unordered_map<int, double>> &C, int w, int i, int v_size){
        MinHeap dpq(v_size);
        dpq.insertKey(w, 0);
        for (int j = 0; j < v_size; j++) {
            if (j != w)
                dpq.insertKey(j, INFINITY);
        }

        vector<double> d_temp(v_size, INFINITY);
        d_temp[w] = 0;
        unordered_set<int> visited_vertices;

        QueueItem q = dpq.extractMin();
        while (q.v != -1){
            if (!visited_vertices.contains(q.v)){
                auto adjecent_vertices = graph[q.v];
                for (auto [key, value]: adjecent_vertices){
                    double tmp_dist = d_temp[q.v] + value.weight;
                    if (tmp_dist < d(i + 1, key)){
                        d_temp[key] = min(d_temp[key], tmp_dist);
                        if (C[w].contains(key)){
                            C[w][key] = d_temp[key];
                        }
                        else{
                            C[w].insert({key, d_temp[key]});
                        }
                        dpq.decreaseKey(key, d_temp[key]);
                    }
                }
                visited_vertices.insert(q.v);
            }
            q = dpq.extractMin();
        }
    }

    std::tuple<int, double> min_dist(int v, const std::unordered_set<int>& a_i, Matrix<double> distances){
        int pi = -1;
        double dist = INFINITY;
        for (int u: a_i){
            double tmp_dist = distances(v, u);
            if (tmp_dist < dist){
                pi = u;
                dist = tmp_dist;
            }
        }
        return {pi, dist};
    }
};

#endif //THESISSOURCE_THORUPZWICKGENERALADO_HPP
