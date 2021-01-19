//
// Created by loenberg on 12/20/20.
//

#ifndef THESISSOURCE_MATRIX_HPP
#define THESISSOURCE_MATRIX_HPP

#include <vector>

using namespace std;

template <typename T>
class Matrix{
private:
    vector<T> data;
    int mRows;
    int mCols;

public:

    Matrix(){

    }

    Matrix(int n, T (*constructor)(int,int)){
        data = vector<T>(n * n);
        mRows = n;
        mCols = n;
        for (int i = 0; i < n * n; i++) {
            data[i] = constructor(i / n, i % n);
        }
    }

    explicit Matrix(int n):Matrix(n, [](int i, int j) {return T();}){
        mRows = mCols = n;
    }

    Matrix(int rows, int cols):Matrix(rows * cols){
        mRows = rows;
        mCols = cols;
    }

    Matrix(int rows, int cols, T (*constructor)(int,int)): Matrix(rows*cols, constructor){
        mRows = rows;
        mCols = cols;
    }

    T& operator()(int i, int j){
        return data[(i * mCols) + j];
    }

    T const& operator()(int i, int j)const{
        return data[(i * mCols) + j];
    }

    [[nodiscard]] int GetRows() const{
        return mRows;
    }

    [[nodiscard]] int GetCols() const{
        return mCols;
    }
};

#endif //THESISSOURCE_MATRIX_HPP
