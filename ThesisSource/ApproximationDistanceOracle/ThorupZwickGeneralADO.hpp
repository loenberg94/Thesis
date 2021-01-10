//
// Created by loenberg on 12/19/20.
//



#ifndef THESISSOURCE_THORUPZWICKGENERALADO_HPP
#define THESISSOURCE_THORUPZWICKGENERALADO_HPP

#include "../BaseClasses/DistanceOracle.hpp"
#include "../HelperTypes/Matrix.hpp"
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
    explicit ThorupZwickGeneralADO(int k, int[] v, int v_size, Matrix<double> distances) {
        bunches = std::unordered_map<int, double>[v_size];
        prepro(k, v, v_size, distances);
    }

    double GetDistance(int u, int v) override{
        return 0;
    }

    int GetSize() override {
        return 0;
    }

private:
    std::unordered_map<int, double> bunches[];
    Matrix<int> p;
    Matrix<double> d;

    static void prepro(int k, int[] v, int v_size, Matrix<double> distances){
        std::random_device rd;
        auto gen = std::mt19937 (rd());
        std::uniform_real_distribution<double> dis= std::uniform_real_distribution<double> (0,1);

        std::unordered_set<int> A[k + 1];

        p = Matrix<int> (k, v_size, [](int i, int j) {return 0;});
        d = Matrix<double> (k + 1, v_size, [](int i, int j) {return 0.0;});

        A[0] = std::unordered_set<int> (v);
        A[k] = std::unordered_set<int> ();

        std::list<int> a_differences[k];


        for (int i = 1; i < k; i++) {
            auto s = A[i - 1];
            for (int elem: s){
                auto prob = dis(gen);
                if (prob <= std::pow((double)v, -(1.0/(double)k))){
                    A[i].insert(elem);
                }
            }
        }

        for (int i = 0; i < k - 1; i++) {
            for (int elem: A[i]) {
                if(!A[i + 1].contains(elem)){
                    a_differences[i].insert(elem);
                }
            }
        }
        a_differences[k] = std::list<int> (A[k - 1]);

        for (int v_: v){
            for (int i = 0; i < k; i++) {
                const auto [pi, dist] = min_dist(v_, A[i], distances);
                p(i, v_) = pi;
                d(i, v_) = dist;
            }
            d(k, v_) = MAXFLOAT;
        }

        for (int v_: v){
            for (int i = 0; i < k; i++) {
                for (int w: a_differences){
                    double tmp_dist = distances(w, v_);
                    if(tmp_dist < d(i + 1, v_)){
                        bunches[v_].insert(w, tmp_dist);
                    }
                }
            }
        }

    }

    static std::tuple<int, double> min_dist(int v, const std::list<int>& a_i, Matrix<double> distances){
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

    int[] range(int start, int end){
        int size = (end - start) + 1;
        int r[size];
        for (int i = 0; i < size; i++) {
            r[i] = start + i;
        }
        return r;
    }
};

#endif //THESISSOURCE_THORUPZWICKGENERALADO_HPP
