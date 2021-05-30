#ifndef STETSEGBETTER_H_
#define STETSEGBETTER_H_

#include<vector>
#include<memory>
#include<queue>
#include<stack>
#include<map>
#include<functional>
#include<iostream>
using std::vector;
using std::cout;
using std::endl;

namespace StetAlgo{

using Interval = std::pair<size_t,size_t>;

std::ostream& operator<<(std::ostream& o,const Interval& p){
    o << "[" << p.first << ","<<p.second << "]"; 
    return o;
}

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
    int N,
    typename T,
    typename ReducerType,
    typename CheckpointType
    >
class PersistentSegTree{
    public:
    using indexType = int;

    private:
    struct Node{
        T val;
        CheckpointType version;
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        bool terminal=false;
        void print(){
            cout << "val: " << val <<endl;
            cout << "version: " << version << endl;
            cout << "l: "<<left << "/r: " << right << endl;
            cout << "terminal: " << terminal << endl;
        }
    };
    indexType treeSize;
    
    ReducerType reducer;
    CheckpointType currentCheckpoint;
    std::map<CheckpointType, std::shared_ptr<Node> > checkpoints;

    public:
    PersistentSegTree(const std::vector<T>& _container, 
            ReducerType _reducer, 
            CheckpointType _currentCheckpoint, 
            T _defaultValue)
        : treeSize(powerOfTwoGreaterOrEqualTo(N)),
          reducer{_reducer}, currentCheckpoint{_currentCheckpoint} 
    {
        checkpoints[currentCheckpoint] = std::make_shared<Node>();
        std::queue<Interval> it; it.push({0,treeSize-1});
        std::queue<std::shared_ptr<Node> > Q; Q.push( checkpoints[currentCheckpoint] );
        std::stack<std::shared_ptr<Node> > S;
        while(!Q.empty()){
            std::shared_ptr<Node> nowNode = Q.front(); Q.pop();
            Interval nowInterval = it.front(); it.pop();

            nowNode->version = currentCheckpoint;

            if(nowInterval.first == nowInterval.second){
                nowNode->terminal = true;
                if(nowInterval.first < _container.size()){
                    nowNode->val = _container[nowInterval.first];
                } else {
                    nowNode->val = _defaultValue;
                }
            }
            else{
                S.push(nowNode); // val, later on.
                nowNode->terminal = false;
                nowNode->left = std::make_shared<Node>();
                nowNode->right = std::make_shared<Node>();
                Q.push(nowNode->left); Q.push(nowNode->right);
                it.push(leftHalf(nowInterval)); it.push(rightHalf(nowInterval));
            }
        }
        while(!S.empty()){
            std::shared_ptr<Node> nowNode = S.top(); S.pop();
            nowNode->val = reducer(nowNode->left->val,nowNode->right->val);
        }
    }
    
    void makeNewLeft(std::shared_ptr<Node> node){
        node->left = std::make_shared<Node>( *(node->left) );
        node->left->version = node->version;
    }
    void makeNewRight(std::shared_ptr<Node> node){
        node->right = std::make_shared<Node>( *(node->right) );
        node->right->version = node->version;
    }
    void update(indexType position, const T& value){
        Interval now = {0,treeSize-1};
        std::shared_ptr<Node> nowNode = checkpoints[currentCheckpoint];
        std::stack< std::shared_ptr<Node> > S; 
        while( ! nowNode->terminal ){
            S.push(nowNode);
            if( position <= (now.first + now.second) / 2){
                if(nowNode->version != nowNode->left->version){
                    makeNewLeft(nowNode);
                }
                nowNode = nowNode->left; now = leftHalf(now);
            }
            else{
                if(nowNode->version != nowNode->right->version){
                    makeNewRight(nowNode);
                }
                nowNode = nowNode->right; now = rightHalf(now);
            }
        }
        //up the chain:
        nowNode->val = value;
        while(!S.empty()){
            auto now = S.top(); S.pop();
            now->val = reducer(now->left->val,now->right->val);
        }
    }

    T reduce(Interval target, CheckpointType chk){
        std::queue<std::shared_ptr<Node> > Q; Q.push( checkpoints[chk] );
        std::queue<Interval> it; it.push({0,treeSize-1});
        bool untarnished = true;
        T ret;
        while(!Q.empty()){
            auto nowNode = Q.front(); Q.pop();
            Interval nowInterval = it.front(); it.pop();
            if(rgeq(nowInterval, target) ){
                if(untarnished) {ret = nowNode->val; untarnished=false;}
                else ret=reducer(ret,nowNode->val);
            }
            else{
                if( ! noOverlap(leftHalf(nowInterval),target)){
                    Q.push(nowNode->left); it.push(leftHalf(nowInterval));
                }
                if( ! noOverlap(rightHalf(nowInterval),target)){
                    Q.push(nowNode->right); it.push(rightHalf(nowInterval));
                }
            }
        }
        return ret;
    }
    T get(indexType target, CheckpointType chk){
        return reduce({target,target},chk);
    }
    
    void checkout(CheckpointType checkpointName, bool overwrite){
        if(checkpoints.count(checkpointName)>=1 && overwrite==false){
            currentCheckpoint = checkpointName;
        }
        else{
            auto oldRoot = checkpoints[currentCheckpoint];
            checkpoints[checkpointName] = std::make_shared<Node>( *oldRoot );
            checkpoints[checkpointName]->version = checkpointName;
            currentCheckpoint = checkpointName;
        }
    }
    auto getRootNode(CheckpointType chk){
        return checkpoints[chk];
    }
};


template<
    int N,
    typename T,
    typename ReducerType,
    typename CheckpointType
>
auto makePersistentSegTree(const vector<T>& _V, 
                               const ReducerType& _reducer, 
                               const CheckpointType& _initCheckpoint, 
                               const T& _defaultVal = T())
{
    return PersistentSegTree<
                N,
                T,
                ReducerType,
                CheckpointType
                    >
                    (_V,_reducer, _initCheckpoint,_defaultVal);
}

} // namespace
#endif
