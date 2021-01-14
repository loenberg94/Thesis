//
// Created by loenberg on 12/19/20.
//



#ifndef THESISSOURCE_THORUPZWICKGENERALADO_HPP
#define THESISSOURCE_THORUPZWICKGENERALADO_HPP

#include "../BaseClasses/DistanceOracle.hpp"
#include "../HelperTypes/Matrix.hpp"
#include "../HelperTypes/Edge.hpp"
#include "../HelperTypes/HashTable2Level.hpp"
#include <cassert>
#include <set>
#include <iterator>
#include <cmath>
#include <random>
#include <tuple>
#include <list>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class ThorupZwickGeneralADO: public DistanceOracle {
public:
    ThorupZwickGeneralADO(int k, int v_size, Matrix<double> distances)
    :ThorupZwickGeneralADO(k, v_size){
        prepro_metric(k, v_size, distances);
    }

    ThorupZwickGeneralADO(int k, int v_size, Edge *edges)
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
        return d(w, u) + bunches[v][w];
    }

    int GetSize() override {
        return size;
    }

protected:
    std::unordered_map<int, double> *bunches;
    Matrix<int> p;
    Matrix<double> d;
    int k_;

private:
    int size = 0;

    ThorupZwickGeneralADO(int k, int v_size){
        p = Matrix<int> (k, v_size, [](int i, int j) {return 0;});
        d = Matrix<double> (k + 1, v_size, [](int i, int j) {return 0.0;});
        bunches = new std::unordered_map<int, double>[v_size];
        k_ = k;
    }

    void prepro_metric(int k, int v_size, Matrix<double> distances){
        std::random_device rd;
        auto gen = std::mt19937 (rd());
        std::uniform_real_distribution<double> dis= std::uniform_real_distribution<double> (0,1);

        std::list<int> a_differences[k];
        std::unordered_set<int> A[k + 1];

        for (int i = 0; i < v_size; i++) {
            A[0].insert(i + 1);
        }


        for (int i = 1; i < k; i++) {
            auto s = A[i - 1];
            for (int elem: s){
                auto prob = dis(gen);
                if (prob <= std::pow((double)v_size, -(1.0/(double)k))){
                    A[i].insert(elem);
                }
            }
        }

        for (int i = 0; i < k; i++) {
            for (int elem: A[i]) {
                if(!A[i + 1].contains(elem)){
                    a_differences[i].push_back(elem);
                }
            }
        }

        for (int v = 0; v < v_size; v++){
            for (int i = 0; i < k; i++) {
                const auto [pi, dist] = min_dist(v, A[i], distances);
                p(i, v) = pi;
                d(i, v) = dist;
            }
            d(k, v) = MAXFLOAT;
        }

        for (int v = 0; v < v_size; v++){
            for (int i = 0; i < k; i++) {
                for (int w: a_differences[i]){
                    double tmp_dist = distances(w, v);
                    if(tmp_dist < d(i + 1, v)){
                        bunches[v].insert({w, tmp_dist});
                        size++;
                    }
                }
            }
        }

    }

    void prepro_graph(int k, int v_size, Edge *edges){

    }

    std::tuple<int, double> min_dist(int v, const std::unordered_set<int>& a_i, Matrix<double> distances){
        int pi = -1;
        double dist = MAXFLOAT;
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
