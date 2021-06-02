#ifndef RANDINT_HPP_
#define RANDINT_HPP_

#include<random>
#include<chrono>
#include<climits>
#include<vector>

template<typename T>
class RNG{
    //TODO extend to integral types
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<T> dis;
    public:
    RNG(T a=INT_MIN,T b=INT_MAX)
        : gen(rd()), dis(a,b)
    {}
    T operator()(){
        return dis(gen);
    }
};


#endif
