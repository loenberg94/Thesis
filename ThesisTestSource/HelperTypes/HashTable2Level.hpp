//
// Created by loenberg on 1/11/21.
//

#ifndef THESISTESTSOURCE_HASHTABLE2LEVEL_HPP
#define THESISTESTSOURCE_HASHTABLE2LEVEL_HPP

#include "Matrix.hpp"
#include <cmath>
#include <random>
#include <vector>
#include "PrimeGenerator.hpp"

class HashTable2Level{
public:
    HashTable2Level(int *elements, int size){
        mSize = size;
        mData = new int*[size];
        abs = Matrix<long long int>(mSize, 2);
        p = pick_p();
        dis = std::uniform_int_distribution<long long int> (1,p);
        init_ab();
        prepro(elements);
    }

private:
    std::random_device rd;
    std::mt19937 gen = std::mt19937 (rd());
    std::uniform_int_distribution<long long int> dis;

    unsigned long long int p;
    unsigned long long int a,b;

    Matrix<long long int> abs;

    int **mData;
    int mSize;

    void prepro(int *elements){
        int ns[mSize];
        std::vector<int> elems[mSize];
        for (int i = 0; i < mSize; i++) {
            int index = hash(elements[i],a,b);
            ns[index]++;
            elems[index].push_back(elements[i]);
        }
        for (int i = 0; i < mSize; i++) {
            if (ns[i] > 0){
                int m = (int) pow(ns[i], 2.0);
                mData[i] = new int[m];
                std::fill_n(mData[i], m, -1);
            }
        }
        for (int i = 0; i < mSize; i++) {
            int si = hash(elements[i], a, b);
            auto [a_, b_] = pick_ab();
            int mi = hash(elements[i], a_, b_);
            if (mData[si][mi] == -1)
                mData[si][mi] = elements[i];
            else
                i--;
        }
    }

    int hash(int x, int a, int b){
        return ((a * x + b) % p) % mSize;
    }

    void init_ab(){
        a = dis(gen);
        b = dis(gen) - 1;
    }

    std::tuple<long long int, long long int> pick_ab(){
        return {dis(gen), dis(gen) - 1};
    }

    unsigned long long int pick_p(){
        return generate_prime_number(mSize);
    }

};

#endif //THESISTESTSOURCE_HASHTABLE2LEVEL_HPP
