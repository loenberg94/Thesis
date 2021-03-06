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
#include "../HelperTypes/Dijkstra.hpp"
#include "../HelperTypes/Logger.hpp"


#include <cassert>
#include <memory>
#include <set>
#include <iterator>
#include <execution>
#include <algorithm>
#include <cmath>
#include <random>
#include <tuple>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <omp.h>

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
        while (!bunches[v].contains(w)){
            i++;
            int tmp = v;
            v = u;
            u = tmp;
            w = p(i, u);
        }
        double d1 = d(i, u);
        double d2 = bunches[v][w];
        return d1 + d2;
    }

    int GetSize() override {
        return size;
    }

    void calculate_size(){
        if (size == 0){
            size = 2 * (k_ * n);
            for (const auto& b : bunches)
                size += b.size();
        }
    }

protected:
    vector<unordered_map<int, double>> bunches;
    Matrix<int> p;
    Matrix<double> d;
    int k_;
    int n;

private:
    int size = 0;

    ThorupZwickGeneralADO(int k, int v_size):
    p(k, v_size),
    d(k + 1, v_size),
    bunches(v_size),
    n(v_size){
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
                        bunches[v][w] = tmp_dist;
                        size++;
                    }
                }
            }
        }
    }

    void prepro_graph(int k, int v_size, vector<Edge> &edges){
        AdjecencyMatrix adj(v_size, edges);
        std::random_device rd;
        std::mt19937 gen (rd());
        std::uniform_real_distribution<double> dis (0,1);
        std::vector<int> a_differences[k];
        std::unordered_set<int> A[k + 1];

        log("prepro - initial setup");
        for (int v = 0; v < v_size; v++) {
            A[0].insert(v);
            d(k, v) = INFINITY;
        }

        log("prepro - A_0 and d_k setup");

        auto probability_picked = std::pow((double)v_size, -1.0/(double)k);
        for (int i = 1; i < k; i++) {
            auto s = A[i - 1];
            for (int elem: s){
                auto event = dis(gen);
                if (event <= probability_picked){
                    A[i].insert(elem);
                }
                else{
                    a_differences[i - 1].push_back(elem);
                }
            }
            log("prepro - A[" + to_string(i) + "].size: " + to_string(A[i].size()));
        }

        for (int v : A[k - 1])
            a_differences[k - 1].push_back(v);

        log("prepro - A_i and differences setup");

        vector<unordered_map<int, double>> C(v_size);
        auto dijkstra = new Dijkstra(v_size + 1);

        for (int i = k - 1; i >= 0; i--) {
            log("");
            log("prepro - Covers - iteration count");
            adj.add_row(build_new_row(A[i]));

            dijkstra->shortest_distances_with_w(v_size, adj, A[i]);

            auto d_ = dijkstra->get_d();
            auto p_ = dijkstra->get_prev();

            for (int v = 0; v < v_size; v++) {
                d(i, v) = d_[v];
                if (d(i,v) == d(i+1, v))
                    p(i, v) = p(i+1,v);
                else
                    p(i, v) = p_[v];
            }
            log("prepro - Covers - p, d updated");

            adj.remove_last_row();

            log("prepro - Covers - Growing tree");
            log("prepro - A[" + to_string(i) + "] - A[" + to_string(i+1) + "]: " + to_string(a_differences[i].size()));

            #pragma omp parallel for num_threads(std::thread::hardware_concurrency() - 1) default(none) shared(i, v_size, C, a_differences, adj)
            for (int w = 0; w < a_differences[i].size(); w++){
                grow_shortest_dist_tree(adj, C, a_differences[i][w],i, v_size);
            }
            log("prepro - Covers - Tree grown");
        }

        delete dijkstra;

        log("\nprepro - creating bunches");

        #pragma omp parallel for num_threads(std::thread::hardware_concurrency() - 1) default(none) shared(C, v_size)
        for (int v = 0; v < v_size; v++) {
            for (int w = 0; w < v_size; w++) {
                if (C[w].contains(v)){
                    bunches[v][w] = C[w][v];
                }
            }
        }

        log("prepro - bunches created\n");
    }

    vector<Entry> build_new_row(std::unordered_set<int> &A_i){
        vector<Entry> new_row(A_i.size());
        int i = 0;
        for (int w : A_i){
            new_row[i] = {w, 0};
            i++;
        }
        return new_row;
    }

    void grow_shortest_dist_tree(AdjecencyMatrix &graph, vector<unordered_map<int, double>> &C, int w, int i, int v_size){
        MinHeap dpq(v_size);
        dpq.insertKey(w, 0);
        for (int j = 0; j < v_size; j++) {
            if (j != w)
                dpq.insertKey(j, INFINITY);
        }

        vector<double> d_temp(v_size, INFINITY);
        vector<bool> visited(v_size, false);
        d_temp[w] = 0;

        QueueItem q = dpq.extractMin();
        while (q.v != -1){
            if (!visited[q.v]){
                auto adjecent_vertices = graph[q.v];
                for (auto edge: adjecent_vertices){
                    if (visited[edge.v])
                        continue;
                    double tmp_dist = d_temp[q.v] + edge.weight;
                    if (tmp_dist < d(i + 1, edge.v)){
                        d_temp[edge.v] = min(d_temp[edge.v], tmp_dist);
                        C[w][edge.v] = d_temp[edge.v];
                        dpq.decreaseKey(edge.v, d_temp[edge.v]);
                    }
                }
                visited[q.v] = true;
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
