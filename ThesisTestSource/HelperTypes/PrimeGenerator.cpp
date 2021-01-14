//
// Created by loenberg on 1/14/21.
//

#include "PrimeGenerator.hpp"
#include <random>
#include <cmath>

std::random_device rd;
std::mt19937 gen = std::mt19937 (rd());
std::uniform_int_distribution<long long int> is_prime_dis;
std::uniform_int_distribution<long long int> candidate_dis;

bool is_prime(long long int n, int k = 128){
    is_prime_dis = std::uniform_int_distribution<long long int> (2,n - 1);
    if (n == 2 || n == 3)
        return true;

    if (n <= 1 || n % 2 == 0)
        return false;

    long long int s = 0, r = n - 1;
    while ((r & 1) == 0){
        s++;
        r /= 2;
    }
    for (int i = 0; i < k; i++) {
        long long int a = is_prime_dis(gen);
        long long int x = (long long int)(powl(a, r)) % n;
        if (x != 1 && x != n - 1){
            long long int j = 1;
            while (j < s && x != n-1){
                x = (long long int)(powl(x,2)) % n;
                if (x == 1)
                    return false;
                j++;
            }
            if (x != n - 1)
                return false;
        }
    }

    return true;
}

long long int generate_prime_candidate(){
    long long int p = candidate_dis(gen);
    if (p % 2 == 0)
        p++;
    return p;
}

long long generate_prime_number(long long int min){
    candidate_dis = std::uniform_int_distribution<long long int> (min);
    long long int p = 4;
    while (!is_prime(p)){
        p = generate_prime_candidate();
    }
    return p;
}

