#ifndef MO_HPP_
#define MO_HPP_

#include<algorithm>

namespace StetAlgo{

class MoCrit{
    long long S;
public:
    MoCrit(long long _S)
        :S{_S}
    {}
    bool operator()(std::pair<int,int> x,std::pair<int,int> y) const{
        if(x.first / S != y.first / S) return x.first < y.first;
        return x.second < y.second;
    }
};

}
#endif
