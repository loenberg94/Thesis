//
// Created by loenberg on 1/14/21.
//

#ifndef THESISTESTSOURCE_ADJECENCYMATRIX_HPP
#define THESISTESTSOURCE_ADJECENCYMATRIX_HPP

#include <vector>
#include <cmath>

using namespace std;

struct Entry{
    int v;
    double weight;
};

class AdjecencyMatrix{
private:
    vector<vector<Entry>> mData;

public:
    AdjecencyMatrix(int v, vector<Edge> &edges):
        mData(v)
    {
        for (auto e: edges){
            mData[e.u].push_back({e.v, e.weight});
        }
    }

    vector<Entry>& operator[](int i){
        return mData[i];
    }
    Entry& operator()(int i, int j){
        return mData[i][j];
    }
};

#endif //THESISTESTSOURCE_ADJECENCYMATRIX_HPP
