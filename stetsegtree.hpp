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
        std::function<void(const T&)> updateValue;
        public:
        ref(std::function<T()> _getValue,std::function<void(const T&)> _updateValue)
            : getValue{_getValue}, updateValue{_updateValue}
        operator T(){ return getValue(); }
        void operator=(const T& other){ updateValue(other); }
    }; // class StetSegTree::ref

    struct Node{
        Interval interval;
        T val;
        size_t leftidx  = -1;
        size_t rightidx = -1;
        size_t parent
        OperationIDType pending;
        Node(Interval _i,T _val, size_t _parent)
            : interval{_i},val{_val},parent{_parent},pending{} 
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


    StetSegTree(const vector<T>& _V, size_t _treeSize, size_t defaultValue,
                ReducerType _reducer,
                OperationType _operation,
                OperationComposerType _composer,
                CheckpointType _initCheckpoint)
        : treeSize{powerOfTwoGreaterOrEqualTo(_treeSize)},
          reducer{_reducer},
          operation{_operation},
          composer{_composer},
          currentCheckpoint{_initCheckpoint},
    {
        //DYNAMIC - create nodes and paths to existing variables
        Interval interval {0, treeSize-1};
        std::queue<size_t> Q;

        nodes.emplace_back(interval, T(), 0); Q.push(0);
        checkpoints[currentCheckpoint] = &nodes[0];
        while(!Q.empty()){
            size_t b = Q.front(); Q.pop();
            if(nodes[b].interval.first == nodes[b].interval.second) continue;
            nodes.emplace_back(leftHalf(nodes[b].interval), T(), b);
            nodes[b].leftidx = nodes.size()-1;
            Q.push( nodes.size() - 1);
            nodes.emplace_back(rightHalf(nodes[b].interval), T(), b);
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
            else x.val = reducer(nodes[x.leftidx].val,nodes[x.rightidx].val);
        }

    }//Constructor
    size_t getRootIndex(CheckpointType chk=currentCheckpoint){
        return checkpoints[chk]->parent; // parent of root is itself
    }
    size_t intervalToNodeIndex(Interval target){
        size_t nowIndex = getRootIndex();
        while(node[nowIndex].interval != target){
            if(noOverlap(leftHalf(nodes[nodeIndex]),target)){
                nowIndex = nodes[nowIndex].leftidx;
            }
            else { nowIndex = nodes[nowIndex].rightidx; }
        }
        return nowIndex;
    }
    void propagate(size_t NodeIndex){
        Node& x = nodes[NodeIndex];
        if( OperationIDType() == x.val ) return;
        x.val = operation(x.Interval, x.val, x.pending);
        if( x.interval.left == x.interval.right ) return;
        nodes[x.right].pending = composer(x.pending, nodes[x.right].pending);
        nodes[x.left].pending = composer(x.pending, nodes[x.left].pending);
        x.pending = OperationIDType();
    }
    void propagateDownTo(size_t NodeIndex){
        size_t nowIndex = getRootIndex();
        Interval target = nodes[NodeIndex].interval;
        while(nowIndex != NodeIndex){
            propagate(nowIndex);
            if(noOverlap(leftHalf(nodes[nowIndex]),target)){
                nowIndex = nodex[nowIndex].leftidx; 
            }
            else { nowIndex = nodes[nowIndex].rightidx; }
        }
    }

    T getValue(size_t NodeIndex){
        propagateDownTo(NodeIndex); return nodes[NodeIndex].val;
    }

    void updateValue(size_t NodeIndex, const T& value){
        propagateDownTo(nodes[NodeIndex].interval);
        nodes[NodeIndex] = value;
        while(NodeIndex.parent!=NodeIndex){ // parent of root is itself
             NodeIndex = NodeIndex.parent;
             Node& x = nodes[NodeIndex.parent];
             x.val = reduce(getValue(x.leftidx),getValue(x.rightidx));
        }
    }

    ref operator[](size_t nodeNum){
        size_t nodeIndex = nodeNumToNodeIndex(nodeNum);
        return ref([this,nodeIndex](){return getValue(nodeIndex);},
                   [this,nodeIndex](const T& value){return updateValue(nodeIndex,value);}
                );
    }

    T reduce(Interval target, CheckpointType chk){
        std::queue<size_t> Q; Q.push( getRootIndex(chk) );
        bool untarnished = true;
        T ret;
        while(!Q.empty()){
            size_t nowIndex = Q.front(); Q.pop();
            propagate(nowIndex);
            if( rgeq(nodes[nowIndex].interval,target) ){
                if(untarnished) ret = nodes[nowIndex].val;
                else ret = reducer(ret,nodes[nowIndex].val);
            }
            else{ //overlapped, but not quite "in" there.
                if( ! noOverlap( leftHalf(nodes[nowIndex]),target ) ){
                    Q.push( nodes[nowIndex].leftidx );
                }
                if( ! noOverlap( rightHalf(nodes[nowIndex]),target ) ){
                    Q.push( nodes[nowIndex].rightidx );
                }
            }
        }
        return ret;
    }

    void updateInterval(Interval target,std::function<T(Interval,const T&,operationIDType)> op){
    }
}


}//namespace StetAlgo


#endif
