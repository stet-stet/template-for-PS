#ifndef SEGTREE_H_
#define SEGTREE_H_

#include<vector>
#include<functional>
using std::vector;

namespace StetAlgo{

constexpr size_t powerOfTwoGreaterOrEqualTo(size_t N){
    N--;
    size_t ret=0;
    while(N!=0){
        N/=2; ++ret;
    }
    return 1<<ret;
}

//TODO: make a factory function for this class.
template<typename T,typename Reducer>
class SegTree{
    /*
Class Invariables:
 container[i] = reducer(container[2*i], container[2*i+1])
       */
    using Interval = std::pair<size_t,size_t>;

    size_t arraysize;
    std::vector<T> container;
    Reducer reducer;
    T fallbackValue;

    class ref{
        std::function<void(const T&)> toCall; // TODO: don't use std::function
        std::function<T()> getValue; // TODO: don't use std::function
        public:
        ref(std::function<void(const T&)> _toCall, std::function<T()> _getValue)
            : toCall{_toCall}, getValue{_getValue}
        {  }
        operator T() const {
            return getValue();
        }
        ref& operator=(const T& other){
            toCall(other); return *this;
        }
        // no other operators will be implemented
    };
    T _reduceInterval(size_t l,size_t r){
        //l,r are **container indices**, unlike all other functions here.
        T ret;
        bool untarnished = true;
        if(l==r) return container[l];
        else if(l>r) return fallbackValue;
        while(l<=r){
            if(l%2==1){
                if(untarnished) {ret = container[l++]; untarnished=false;}
                else ret = reducer(ret,container[l++]);
            }
            if(r%2==0){
                if(untarnished) {ret = container[r--]; untarnished=false;}
                else ret = reducer(ret,container[r--]);
            }
            l/=2;r/=2;
        }
        return ret;
    }
public:
    // TODO: make the compiler spew "appropriate" compile-time errors if C is not:
    //  1. a STL container containing a bunch of Ts
    //  2. an iterable
    // TODO: make the compiler spew "appropriate" compile-time errors if reducer does not
    //  take two Ts and returns another T.
    SegTree(const vector<T>& _container,Reducer _reducer, size_t N, T _fallback=T())
     : container(2*powerOfTwoGreaterOrEqualTo(N)), reducer {_reducer}, fallbackValue{_fallback}
    {
        arraysize = powerOfTwoGreaterOrEqualTo(N);
        size_t i=0;
        for(size_t i=0;i<arraysize;++i){
            if( i < _container.size()) container[i+arraysize] = _container[i];
            else container[i+arraysize] = fallbackValue;
        }
        for(size_t i=arraysize-1;i>0;--i){
            container[i] = reducer(container[2*i],container[2*i+1]);
        }
    }

    void update(size_t location,const T& value){
        size_t idx = location + arraysize;
        container[idx] = value; idx/=2;
        while(idx>0){
            container[idx] = reducer(container[2*idx],container[2*idx+1]);
            idx/=2;
        }
    }

    T getValue(size_t location){
        return container[location+arraysize];
    }

    T reduce(Interval interval){
        return _reduceInterval(interval.first+arraysize,interval.second+arraysize);
    }

    ref operator[](size_t index){
        return ref(
                [this,index](const T& val){update(index,val); }, //toCall
                [this,index](){return getValue(index);} //getValue
                );
    }
};

template<size_t N, typename T, typename Reducer>
auto makeSegTree(const vector<T>& _V, const Reducer& reducer, const T& _fallbackVal = T()){
    return SegTree<T,Reducer>(_V, reducer, N, _fallbackVal);
}

}

#endif
