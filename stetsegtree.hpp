#ifndef STETSEGTREE_H_
#define STETSEGTREE_H_
#include<memory>
#include<vector>
#include<queue>
#include<map>
#include<functional>
#include<iostream>
using std::vector;
using std::cout;
using std::endl;


namespace StetAlgo{

// refactor these away later on, if possible!
using Interval = std::pair<size_t,size_t>;

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
constexpr bool rgeq(Interval a,Interval b){
    return b.first<=a.first && a.second<=b.second;
}
Interval intersection(Interval a,Interval b){
    if(noOverlap(a,b)) return {-1,-1};
    else if(rgeq(a,b)) return a;
    else if(rgeq(b,a)) return b;
    else return {std::max(a.first,b.first),std::min(a.second,b.second)};
}

template<
    typename T,
    typename ReducerType,
    typename OperationIDType,
    typename OperationType,
    typename OperationComposerType,
    typename CheckpointType
    >
class StetSegTree{
    private:
    class ref{
        std::function<T()> getValue;
        std::function<void(const T&)> updateValue;
        public:
        ref(std::function<T()> _getValue,std::function<void(const T&)> _updateValue)
            : getValue{_getValue}, updateValue{_updateValue} 
        {}
        operator T(){ return getValue(); }
        void operator=(const T& other){ updateValue(other); }
    }; // class StetSegTree::ref

    struct Node{
        Interval interval;
        T val;
        size_t leftidx  = -1;
        size_t rightidx = -1;
        size_t parent;
        OperationIDType pending;
        CheckpointType version;
        Node(Interval _i,T _val, size_t _parent,CheckpointType version)
            : interval{_i},val{_val},parent{_parent},pending{} 
        {}
    }; //struct Node

    class ConstTreeIterator{
        size_t nodeNum;
        vector<Node> *nodes;
        std::function<void(size_t)> onNodeChange; //always called upon change in nodeNum;(except when NONE)
        public:
        ConstTreeIterator(size_t _nodeNum,
                vector<Node> *_nodes,
                const std::function<void(size_t)>& _onNodeChange)
            : nodeNum{_nodeNum}, nodes{_nodes}, onNodeChange{_onNodeChange}
        {
            //this will only be initialized with a root node (whichever it may be)
            onNodeChange(nodeNum);
        }
        T operator*(){ return (*nodes)[nodeNum].val; } 
        bool isTerminal(){ return (*nodes)[nodeNum].interval.first == (*nodes)[nodeNum].interval.second; }
        Interval getInterval(){ return (*nodes)[nodeNum].interval;}
        ConstTreeIterator left(){
            return ConstTreeIterator( (*nodes)[nodeNum].leftidx, nodes, onNodeChange );
        }
        ConstTreeIterator right(){
            return ConstTreeIterator( (*nodes)[nodeNum].rightidx, nodes, onNodeChange);
        }
        bool operator==(const ConstTreeIterator& other){
            return nodeNum==other.nodeNum;
        }
        bool operator!=(const ConstTreeIterator& other){
            return nodeNum!=other.nodeNum;
        }
        void print() const{
            cout << "node " << nodeNum << endl;
            cout << "left: "<< (*nodes)[nodeNum].leftidx << "/right: " << (*nodes)[nodeNum].rightidx << endl;
            cout << "interval: {" << (*nodes)[nodeNum].interval.first << " " << (*nodes)[nodeNum].interval.second<<endl;
            cout << "value: " << (*nodes)[nodeNum].val << endl;
        }
    };
    // variables
    constexpr static size_t NONE = 1ll<<63; // no node
    public:
    // VANILLA
    ReducerType reducer;
    size_t treeSize;
    // LAZINESS
    // Interval, T, OperationIDType -> T
    OperationIDType defaultOpID;
    OperationType operation;
    // OperationIDType, OperationIDType -> OperationIDType
    OperationComposerType composer;
    // PERSISTENCY
    vector<Node> nodes;
    std::map<CheckpointType,size_t> checkpoints;
    CheckpointType currentCheckpoint;

public:
    StetSegTree(const vector<T>& _V, size_t _treeSize, size_t defaultValue,
                const ReducerType& _reducer,
                const OperationIDType& _defaultOpID,
                const OperationType& _operation,
                const OperationComposerType& _composer,
                const CheckpointType& _initCheckpoint)
        : treeSize{powerOfTwoGreaterOrEqualTo(_treeSize)},
          reducer{_reducer},
          defaultOpID{_defaultOpID},
          operation{_operation},
          composer{_composer},
          currentCheckpoint{_initCheckpoint}
    {
        //DYNAMIC - create nodes and paths to existing variables
        Interval interval {0, treeSize-1};
        std::queue<size_t> Q;

        nodes.emplace_back(interval, T(), 0, currentCheckpoint); Q.push(0);
        checkpoints[currentCheckpoint] = 0;
        while(!Q.empty()){
            size_t b = Q.front(); Q.pop();
            if(nodes[b].interval.first == nodes[b].interval.second) continue;
            nodes.emplace_back(leftHalf(nodes[b].interval), T(), b, currentCheckpoint);
            nodes[b].leftidx = nodes.size()-1;
            Q.push( nodes.size() - 1);
            nodes.emplace_back(rightHalf(nodes[b].interval), T(), b, currentCheckpoint);
            nodes[b].rightidx = nodes.size()-1;
            Q.push( nodes.size() - 1);
        }
        for(auto rit=nodes.rbegin();rit!=nodes.rend();++rit){
            Node& x = (*rit);
            if(x.interval.first == x.interval.second){
                size_t a = x.interval.first;
                if( a < _V.size() ){
                    x.val = _V[a];
                } else{
                    x.val = defaultValue;
                }
            }
            else x.val = reducer(nodes[x.leftidx].val,nodes[x.rightidx].val);
        }
    }//Constructor
    size_t getRootIndex(CheckpointType chk){
        return nodes[checkpoints[chk]].parent; // parent of root is itself
    }
    size_t getRootIndex(){
        return getRootIndex(currentCheckpoint);
    }

    void propagate(size_t NodeIndex){
        Node& x = nodes[NodeIndex];     
        if(x.pending == defaultOpID) return;
        x.val = operation(x.interval, x.val, x.pending);
        if( x.interval.first == x.interval.second ){ x.pending = defaultOpID; return;}
        nodes[ nodes[NodeIndex].rightidx ].pending = 
            composer(x.pending, nodes[ nodes[NodeIndex].rightidx ].pending);
        nodes[ nodes[NodeIndex].leftidx ].pending = 
            composer(x.pending, nodes[ nodes[NodeIndex].leftidx ].pending);
        x.pending = defaultOpID;
    }

    void getNewLeft(size_t NodeIndex){
        //only call after you've fiddled with pending val
        propagate(nodes[NodeIndex].leftidx);
        nodes.push_back( nodes[ nodes[NodeIndex].leftidx ]);
        nodes[NodeIndex].leftidx = nodes.size()-1;
        nodes.back().version = nodes[NodeIndex].version;
        nodes.back().parent = NodeIndex;
    }
    void getNewRight(size_t NodeIndex){
        //only call after you've fiddled with pending val
        propagate(nodes[NodeIndex].rightidx);
        nodes.push_back( nodes[ nodes[NodeIndex].rightidx ]);
        nodes[NodeIndex].rightidx = nodes.size()-1;
        nodes.back().version = nodes[NodeIndex].version;
        nodes.back().parent = NodeIndex;
    }


    T reduce(Interval target, CheckpointType chk){
        std::queue<size_t> Q; Q.push( getRootIndex(chk) );
        bool untarnished = true;
        T ret;
        while(!Q.empty())
        {
            size_t nowIndex = Q.front(); Q.pop();
            //cout << "{"<<nodes[nowIndex].interval.first << ", "<< nodes[nowIndex].interval.second << "}";
//            cout << nowIndex << "in reduce" << endl;
            propagate(nowIndex);
            if( rgeq(nodes[nowIndex].interval,target) ){
                if(untarnished){ ret = nodes[nowIndex].val; untarnished=false;}
                else ret = reducer(ret,nodes[nowIndex].val);
            }
            else{ //overlapped, but not quite "in" there.
                if( ! noOverlap( leftHalf(nodes[nowIndex].interval),target ) ){
                    Q.push( nodes[nowIndex].leftidx );
                }
                if( ! noOverlap( rightHalf(nodes[nowIndex].interval),target ) ){
                    Q.push( nodes[nowIndex].rightidx );
                }
            }
        }
        return ret;
    }
    T reduce(Interval target){
        return reduce(target,currentCheckpoint);
    }

    T getValue(size_t nodeNum){
        return reduce({nodeNum,nodeNum});
    }

    void updateValue(size_t NodeNum, const T& value){ // can't this be a part of updateInterval...?
        Interval target {NodeNum,NodeNum};
        size_t nowIndex = getRootIndex();
        while(nodes[nowIndex].interval != target){
            propagate(nowIndex);
            if(rgeq(target,leftHalf(nodes[nowIndex].interval)) ){
                if(nodes[nowIndex].version != nodes[nodes[nowIndex].leftidx].version)
                   getNewLeft(nowIndex);
                nowIndex = nodes[nowIndex].leftidx;
            }
            else if(rgeq(target,rightHalf(nodes[nowIndex].interval))){
                if(nodes[nowIndex].version != nodes[nodes[nowIndex].rightidx].version)
                    getNewRight(nowIndex);
                nowIndex = nodes[nowIndex].rightidx;
            }
        }
        propagate(nowIndex);
        //now go up the chain.
        nodes[nowIndex].val = value;
//        cout << "index in update:" << nowIndex << endl;
//        cout << "value in update:" << value << endl;
        while(nodes[nowIndex].parent != nowIndex){
            size_t parentIndex = nodes[nowIndex].parent;
            propagate(nodes[parentIndex].rightidx);
            propagate(nodes[parentIndex].leftidx);
            nodes[parentIndex].val = reducer(
                    nodes[nodes[parentIndex].leftidx].val,
                    nodes[nodes[parentIndex].rightidx].val
                    );
            nowIndex = parentIndex;
        }
    }

    void updateInterval(Interval target, const OperationIDType& op){
        std::queue<size_t> Q; Q.push( getRootIndex() );
        while(!Q.empty()){
            size_t nowIndex = Q.front(); Q.pop();
            propagate(nowIndex);
            if( rgeq(nodes[nowIndex].interval,target) ){
                nodes[nowIndex].pending = composer(op, nodes[nowIndex].pending);
            }
            else{ //overlapped, but not quite "in" there.
                nodes[nowIndex].val = operation(
                        intersection(nodes[nowIndex].interval, target), nodes[nowIndex].val, op);
                if( ! noOverlap( leftHalf(nodes[nowIndex].interval),target ) ){
                    if(nodes[nowIndex].version != nodes[nodes[nowIndex].leftidx].version)
                        getNewLeft(nowIndex);
                    Q.push( nodes[nowIndex].leftidx );
                }
                if( ! noOverlap( rightHalf(nodes[nowIndex].interval),target) ){
                    if(nodes[nowIndex].version != nodes[nodes[nowIndex].rightidx].version)
                        getNewRight(nowIndex);
                    Q.push( nodes[nowIndex].rightidx );
                }
            }
        }
    }

    void checkout(CheckpointType checkpointName, bool overwrite){
        size_t oldRootIndex = getRootIndex(); propagate(oldRootIndex);
        if(checkpoints.count(checkpointName) >= 1 && overwrite == false){
            currentCheckpoint = checkpointName;
        }
        else{
            nodes.push_back(nodes[oldRootIndex]);
            nodes[nodes.size()-1].version = checkpointName;
            nodes[nodes.size()-1].parent = nodes.size()-1; //itself

            currentCheckpoint = checkpointName;
            checkpoints[checkpointName] = nodes.size()-1;
        
        }
    }

    ref operator[](size_t nodeNum){
        return ref([this,nodeNum](){return getValue(nodeNum);},
                   [this,nodeNum](const T& value){
                        updateValue(nodeNum,value);
                   }
                );
    }

    ConstTreeIterator begin(CheckpointType checkpointName){
        return ConstTreeIterator(
                checkpoints[checkpointName], 
                &nodes,
                [this](size_t idx){ propagate(idx);} 
                );
    }

    ConstTreeIterator begin(){
        return begin(currentCheckpoint);
    }

}; // template class StetSegTree


template<
    size_t _treeSize,
    typename T,
    typename ReducerType
>
auto makeSegTree(const vector<T>& _V, const ReducerType& _reducer, const T& _defaultValue = T() ){
    using defaultOpIDType = size_t;
    size_t defaultOpID = 0;
    auto defaultOperator = [](Interval,T,defaultOpIDType){return defaultOpIDType();};
    auto defaultComposer = [](defaultOpIDType,defaultOpIDType){return defaultOpIDType();};
    using defaultOperatorType = decltype(defaultOperator);
    using defaultComposerType = decltype(defaultComposer);
    return StetSegTree<
            T, 
            ReducerType, 
            defaultOpIDType, 
            defaultOperatorType, 
            defaultComposerType,
            size_t //chekcpointType
        >
        (_V, _treeSize, _defaultValue, _reducer, defaultOpID, defaultOperator, defaultComposer, 0);
}

template<
    size_t _treeSize,
    typename T,
    typename ReducerType,
    typename OperationIDType,
    typename OperatorType,
    typename ComposerType
>
auto makeLazyPropSegTree(const vector<T>& _V, 
                 const ReducerType& _reducer, 
                 const OperationIDType& _defaultOpID,
                 const OperatorType& _operator,
                 const ComposerType& _composer,
                 const T& _defaultValue = T() )
{
    return StetSegTree<
            T,
            ReducerType,
            OperationIDType,
            OperatorType,
            ComposerType,
            size_t
        >
        (_V, _treeSize, _defaultValue, _reducer, _defaultOpID, _operator, _composer, 0);
}

template<
    size_t _treeSize,
    typename T,
    typename ReducerType,
    typename CheckpointType
>
auto makePersistentSegTree(const vector<T>& _V,
                const ReducerType& _reducer,
                const CheckpointType& _initCheckpoint,
                const T& _defaultValue = T() )
{
     using defaultOpIDType = size_t;
    size_t defaultOpID = 0;
   auto defaultOperator = [](Interval,T,defaultOpIDType){return defaultOpIDType();};
    auto defaultComposer = [](defaultOpIDType,defaultOpIDType){return defaultOpIDType();};
    using defaultOperatorType = decltype(defaultOperator);
    using defaultComposerType = decltype(defaultComposer);
    return StetSegTree<
            T, 
            ReducerType, 
            defaultOpIDType, 
            defaultOperatorType, 
            defaultComposerType,
            CheckpointType //chekcpointType
        >
        (_V, _treeSize, _defaultValue, _reducer, defaultOpID, defaultOperator, defaultComposer, _initCheckpoint);

}

}//namespace StetAlgo


#endif
