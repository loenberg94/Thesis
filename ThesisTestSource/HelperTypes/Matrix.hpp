//
// Created by loenberg on 12/20/20.
//

#ifndef THESISSOURCE_MATRIX_HPP
#define THESISSOURCE_MATRIX_HPP

#include <vector>
#include <cassert>

using namespace std;

template <typename T>
class Matrix{
protected:
    vector<T> data;
    int mRows;
    int mCols;

public:

    Matrix(){

    }

    explicit Matrix(int n):
    data(n * n){
        mRows = mCols = n;;
    }

    Matrix(int rows, int cols):
    data(rows * cols){
        mRows = rows;
        mCols = cols;
    }


    T& operator()(int i, int j){
        if (i >= mRows)
            throw std::invalid_argument("index i larger than rows");
        if (j >= mCols)
            throw std::invalid_argument("index j larger than collumns");
        return data[(i * mCols) + j];
    }

    T const& operator()(int i, int j)const{
        if (i >= mRows)
            throw std::invalid_argument("index i larger than rows");
        if (j >= mCols)
            throw std::invalid_argument("index j larger than collumns");
        return data[(i * mCols) + j];
    }

    [[nodiscard]] int GetRows() const{
        return mRows;
    }

    [[nodiscard]] int GetCols() const{
        return mCols;
    }

    T get(int i, int j){
        return this->operator()(i,j);
    }
};

#endif //THESISSOURCE_MATRIX_HPP
