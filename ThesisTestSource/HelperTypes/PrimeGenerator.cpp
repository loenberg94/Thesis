//
// Created by loenberg on 1/14/21.
//

#include "PrimeGenerator.hpp"
#include <random>
#include <cmath>

#define lli long long int

std::random_device rd;
std::mt19937 gen = std::mt19937 (rd());
std::uniform_int_distribution<lli> is_prime_dis;
std::uniform_int_distribution<lli> candidate_dis;

lli mulmod(lli a, lli x, lli n)
{
    lli r=1;
    while (x) {
        if ( (x & 1) == 1 )
            r = a*r % n;
        x >>= 1;
        a = a*a % n;
    }
    return r;
}

lli modulo(lli base, lli e, lli m) {
    lli x = 1;
    lli y = base;
    while (e > 0) {
        if (e % 2 == 1)
            x = (x * y) % m;
        y = (y * y) % m;
        e = e / 2;
    }
    return x % m;
}

bool is_prime(lli p, int k = 128){
    is_prime_dis = std::uniform_int_distribution<long long int> (2,p - 1);

    if (p < 2) {
        return false;
    }
    if (p != 2 && p % 2==0) {
        return false;
    }
    lli s = p - 1;
    while (s % 2 == 0) {
        s /= 2;
    }
    for (int i = 0; i < k; i++) {
        lli a = is_prime_dis(gen) % (p - 1) + 1, temp = s;
        lli mod = modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1) {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0) {
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

