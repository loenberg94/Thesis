//
// Created by loenberg on 1/11/21.
//

#ifndef THESISTESTSOURCE_HASHTABLE2LEVEL_HPP
#define THESISTESTSOURCE_HASHTABLE2LEVEL_HPP

#include "Matrix.hpp"
#include <cmath>
#include <random>
#include <vector>
#include <tuple>
#include <exception>
#include <unordered_map>
#include "PrimeGenerator.hpp"

struct HashTableEntry{
    int key;
    double value;
};

struct SecondLevelTable{
    long long int a, b;
    vector<HashTableEntry> table;
};

class HashTable2Level{
public:
    HashTable2Level(vector<HashTableEntry> elements, int size) :
        mData(size),
        mBinsSizes(size, 0)
    {
        mSize = size;
        p = 1610612741;//pick_p();
        dis = std::uniform_int_distribution<long long int> (1,p);
        init_ab();
        prepro(elements);
    }

    HashTable2Level(HashTable2Level *pLevel) {

    }

    bool contains(int key){
        if (mSize == 0)
            return false;
        int si = hash(key, a, b, mSize);
        if (mBinsSizes[si] == 0)
            return false;
        int mi = hash(key, mData[si].a, mData[si].b, mBinsSizes[si]);
        auto entry = mData[si].table[mi];
        return key == entry.key;
    }

    double operator[](int key){
        int si = hash(key, a, b, mSize);
        if (mBinsSizes[si] == 0)
            invalid_key(key);
        int mi = hash(key, mData[si].a, mData[si].b, mBinsSizes[si]);
        auto entry = mData[si].table[mi];
        if (entry.key == key)
            return entry.value;
        invalid_key(key);
    }

private:
    std::random_device rd;
    std::mt19937 gen = std::mt19937 (rd());
    std::uniform_int_distribution<long long int> dis;

    unsigned long long int p;
    unsigned long long int a,b;

    vector<SecondLevelTable> mData;
    vector<int> mBinsSizes;
    int mSize;

    void set_bin_sizes(vector<HashTableEntry> elements, vector<vector<HashTableEntry>> &binElements){
        for (int i = 0; i < mSize; i++) {
            int index = hash(elements[i].key,a,b, mSize);
            mBinsSizes[index]++;
            binElements[index].push_back(elements[i]);
        }
    }

    void prepro(vector<HashTableEntry> elements){
        vector<vector<HashTableEntry>> binElements(mSize, vector<HashTableEntry>());
        set_bin_sizes(elements, binElements);
        for (int i = 0; i < mSize; i++) {
            int m = (int) pow(mBinsSizes[i], 2.0);
            mBinsSizes[i] = m;
            auto [a_, b_] = pick_ab();
            mData[i] = {a_, b_, vector<HashTableEntry>(m, {-1, -1})};
        }

        for (int i = 0; i < mSize; i++){
            if (!binElements[i].empty()){
                auto bin = binElements[i];
                unordered_map<int, HashTableEntry> indeces;
                bool success = true;
                for (auto e: bin){
                    int mi = hash(e.key, mData[i].a, mData[i].b, mBinsSizes[i]);
                    if (indeces.contains(mi)){
                        auto [a_, b_] = pick_ab();
                        mData[i].a = a_;
                        mData[i].b = b_;
                        success = false;
                        break;
                    }
                    else{
                        indeces.insert({mi, e});
                    }
                }
                if (success){
                    for (auto [k, v]: indeces){
                        mData[i].table[k] = v;
                    }
                }
                else{
                    i--;
                }
            }
        }
    }

    int hash(int x, int a_, int b_, int size){
        return ((a_ * x + b_) % p) % size;
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

    void invalid_key(int k){
        throw std::invalid_argument("Invalid key: " + std::to_string(k));
    }
};


#endif //THESISTESTSOURCE_HASHTABLE2LEVEL_HPP
