//
// Created by loenberg on 1/10/21.
//

#ifndef THESISTESTSOURCE_WULFFNILSENGENERALADO_HPP
#define THESISTESTSOURCE_WULFFNILSENGENERALADO_HPP

#include "ThorupZwickGeneralADO.hpp"
#include <cmath>

class WulffTree{
public:
    int i1, i2;
    vector<int> maximizing_u;
    int even_middle;
    WulffTree* left = nullptr;
    WulffTree* right = nullptr;

    WulffTree(int i1, int i2, int n):
    i1(i1),
    i2(i2),
    maximizing_u(n)
    {
        even_middle = even_middle_index(i1, i2);
    }

private:
    static int even_middle_index(int i1, int i2){
        int m = ((i2 - i1) / 2) + i1;
        if (m % 2 == 0)
            return m;
        return m + 1;
    }

};

class WulffNilsenGeneralADO: public ThorupZwickGeneralADO{
public:
    WulffNilsenGeneralADO(int k, int v_size, Matrix<double> &distances)
        :ThorupZwickGeneralADO(k, v_size, distances){
        root = prepro_tree(0, k - 1, v_size);
    }

    WulffNilsenGeneralADO(int k, int v_size, vector<Edge> &edges)
            :ThorupZwickGeneralADO(k, v_size, edges){
        root = prepro_tree(0, k - 1, v_size);
    }

    double GetDistance(int u, int v) override{
        return bdist(u,v, root);
    }

private:
    WulffTree* root;

    double delta(int i, int u){
        return d(i + 2, u) - d(i, u);
    }

    WulffTree* prepro_tree(int i1, int i2, int n){
        double t = ceil(log2(k_));
        if (i2 - i1 <= t){
            auto leaf = new WulffTree(i1, i2, n);
            for (int u = 0; u < n; u++){
                leaf->maximizing_u[u] = i1;
                for (int i = i1 + 1; i <= i2 - 2; i++){
                    if (i % 2 == 0){
                        if (delta(leaf->maximizing_u[u], u) < delta(i, u))
                            leaf->maximizing_u[u] = i;
                    }
                }
            }
            return leaf;
        }
        else{
            auto node = new WulffTree(i1, i2, n);
            auto left = prepro_tree(i1, node->even_middle, n);
            auto right = prepro_tree(node->even_middle, i2, n);
            node->left = left;
            node->right = right;
            if (!left->right){
                for (int u = 0; u < n; u++)
                    node->maximizing_u[u] = left->maximizing_u[u];
            }
            else{
                for (int u = 0; u < n; u++){
                    if (delta(left->maximizing_u[u], u) > delta(left->right->maximizing_u[u], u))
                        node->maximizing_u[u] = left->maximizing_u[u];
                    else
                        node->maximizing_u[u] = left->right->maximizing_u[u];
                }
            }
            return node;
        }
    }

    double bdist(int u, int v, WulffTree* node){
        if ((node->i2 - node->i1) <= ceil(log2(k_)))
            return dist(u, v, node->i1);
        int j = node->maximizing_u[u];
        if (!bunches[v].contains(p(j, u)) && !bunches[u].contains(p(j+1, v)))
            return bdist(u,v,node->right);
        return bdist(u,v,node->left);
    }

    double dist(int u, int v, int i){
        int w = p(i, u); int j = i;
        while (!bunches[v].contains(w)){
            j++;
            int tmp = v;
            v = u;
            u = tmp;
            w = p(j, u);
        }
        return d(j, u) + bunches[v][w];
    }
};

#endif //THESISTESTSOURCE_WULFFNILSENGENERALADO_HPP
