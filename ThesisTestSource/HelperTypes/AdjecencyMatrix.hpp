//
// Created by loenberg on 1/14/21.
//

#ifndef THESISTESTSOURCE_ADJECENCYMATRIX_HPP
#define THESISTESTSOURCE_ADJECENCYMATRIX_HPP

#include <vector>
#include <cmath>
#include <unordered_map>
#include "Edge.hpp"

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

    void add_row(vector<Entry> row){
        mData.push_back(row);
    }

    void remove_last_row(){
        mData.pop_back();
    }

    void remove_row(int i){
        mData.erase(mData.begin() + i);
    }

    int length() {
        return mData.size();
    }
};

#endif //THESISTESTSOURCE_ADJECENCYMATRIX_HPP
