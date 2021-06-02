#ifndef DISJOINT_SET_H_
#define DISJOINT_SET_H_

#include<vector>
#include<numeric>
namespace StetAlgo{

class DisjointSet{
private:
    std::vector<int> parent;
public:
    DisjointSet(int noOfElements):
        parent(noOfElements + 1)
    { 
        std::iota(parent.begin(),parent.end(),0);
    }
    int find(int a){
        if(parent[a]==a) return a;
        else{
            parent[a] = find(parent[a]); 
            //TODO
            //if something goes wrong,
            //stack overflow caused by this is the reason why.
            return parent[a];
        }
    }
    void add(int a,int b){
        if(find(a)==find(b)) return;
        parent[find(b)] = find(a);
    }
    bool connected(int a,int b){
        return find(a) == find(b);
    }
};

}
#endif
