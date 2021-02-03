//
// Created by loenberg on 1/10/21.
//

#ifndef THESISTESTSOURCE_WULFFNILSENGENERALADO_HPP
#define THESISTESTSOURCE_WULFFNILSENGENERALADO_HPP

#include "ThorupZwickGeneralADO.hpp"
#include <cmath>

class WulffNilsenGeneralADO: public ThorupZwickGeneralADO{
public:
    WulffNilsenGeneralADO(int k, int v_size, Matrix<double> distances)
        :ThorupZwickGeneralADO(k, v_size, distances){
    }

    double GetDistance(int u, int v) override{
        return bdist(u,v, 0, k_ - 1);
    }

private:
    double bdist(int u, int v, int i1, int i2){
        if ((i2 - i1) <= log(k_))
            return dist(u, v, i1);
        int i = even_middle_index(i1, i2);
        int j = even_index_maximize_d(i1, i2);
        if (!bunches[v]->contains(p(j, u)) && !bunches[u]->contains(p(j+1, v)))
            return bdist(u,v,i,i2);
        return bdist(u,v,i1,j);
    }

    double dist(int u, int v, int i){
        int w = p(i, u); int j = i;
        while (!bunches[v]->contains(w)){
            j++;
            int tmp = v;
            v = u;
            u = tmp;
            w = p(j, u);
        }
        return d(w, u) + bunches[w]->operator[](v);
    }

    static int even_middle_index(int i1, int i2){
        int m = ((i2 - i1) / 2) + i1;
        if (m % 2 == 0)
            return m;
        return m + 1;
    }

    static int even_index_maximize_d(int i1, int i2){
        return 0;
    }

    bool is_uv_terminal(int u, int v, int j){
        if (j == k_ - 1)
            return true;
        return (j % 2 == 0) && (bunches[v]->contains(p(j, u)) || bunches[u]->contains(p(j + 1, v)));
    }

    bool is_uv_feasible(int u, int v, int i1, int i2){
        if (i1 % 2 == 0)
            return true;
        if (d(u, p(i1, u)) <= i1 * d(u,v))
            return true;
        return is_uv_terminal(u,v,i2);
    }
};

#endif //THESISTESTSOURCE_WULFFNILSENGENERALADO_HPP
