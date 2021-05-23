#ifndef STETSEGTREE_H_
#define STETSEGTREE_H_

#include<vector>
#include<functional>

namespace StetAlgo{

// refactor these away later on, if possible!
using Interval = std::pair<size_t,size_t>
class LazyType(){};
class PersistentType(){};


constexpr size_t powerOfTwoGreaterOrEqualTo(size_t N){
    --N;
    size_t ret=1;
    while(N!=0){N>>=1;ret<<=1;}
    return ret;
}

constexpr bool noOverlap(Interval a,Interval b){
    return a.second < b.first || b.second < a.first;
}
Interval leftHalf(Interval a){
    return {a.first,(a.second+a.first)/2};
}
Interval rightHalf(Interval a){
    return {(a.second+a.first)/2+1,a.second};
}

template<
    typename T,
    typename ReducerType,
    typename OperationIDType=size_t,
    std::function<T(const Interval&, const T& origVal, const OperationIDType&)> operation,
    std::function<OperationIDType(const OperationIDType&, const OperationIDType&)> composer,
    typename CheckpointType =size_t
    >
class StetSegTree{
    class ref{
        std::function<T()> getValue;
        std::function<void(T)> updateValue;
        public:
        ref(std::function<T()> _getValue,std::function<void(T)> _updateValue)
            : getValue{_getValue}, updateValue{_updateValue}
        operator T(){ return getValue(); }
        void operator=(const T& other){ updateValue(other); }
        void operator=(T&& other){ updateValue(std::move(other)); }
    }; // class StetSegTree::ref

    struct Node{
        Interval interval;
        T val;
        size_t leftidx  = -1;
        size_t rightidx = -1;
        OperationIDType pending;
        Node(Interval _i,T _val)
            : interval{_i},val{_val},pending{} 
        {}
    }; //struct Node

    // variables

    // VANILLA
    ReducerType reducer;
    size_t treeSize;
    // LAZINESS
    // PERSISTENCY
    vector<Node> nodes;
    map<CheckpointType,std::shared_ptr<Node> > checkpoints;
    CheckpointType currentCheckpoint;


    StetSegTree(const vector<T>& _V, size_t _treeSize, const T& _defaultValue,
                ReducerType _reducer,
                OperationType _operation,
                OperationComposerType _composer,
                CheckpointType _initCheckpoint)
        : defaultValue{_defaultValue},
          reducer{_reducer},
          operation{_operation},
          composer{_composer},
          currentCheckpoint{_initCheckpoint}
    {
        treeSize = powerOfTwoGreaterOrEqualTo(_treeSize);
        //DYNAMIC - defaultValueTable

        //DYNAMIC - create nodes and paths to existing variables
        Interval target {0, _V.size()-1};
        Interval interval {0, treeSize-1};
        std::queue<size_t> Q;

        nodes.emplace_back(interval, T()); Q.push(0);
        checkpoints[currentCheckpoint] = &nodes[0];
        while(!Q.empty()){
            size_t b = Q.front(); Q.pop();
            if(nodes[b].interval.first == nodes[b].interval.second) continue;
            nodes.emplace_back(leftHalf(nodes[b].interval), T());
            nodes[b].leftidx = nodes.size()-1;
            Q.push( nodes.size() - 1);
            nodes.emplace_back(rightHalf(nodes[b].interval), T());
            nodes[b].rightidx = nodes.size()-1;
            Q.push( nodes.size() - 1);
        }
        for(size_t i=nodes.size()-1;i>=0;--i){
            Node& x = nodes[i];
            if(x.interval.first == x.interval.second){
                size_t& a = x.interval.first;
                if( a < _V.size() ){
                    x.val = _V[a];
                } else{
                    x.val = defaultVale;
                }
            }
            x.val = reducer(nodes[x.leftidx].val,nodes[x.rightidx].val);
        }

    }//Constructor
}


}//namespace StetAlgo


#endif
