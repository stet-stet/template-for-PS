#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include<vector>
#include<queue>
#include<stack>
#include<iostream>
#include<unordered_map>
#include<climits>
#include<algorithm>
#include"disjointset.hpp"
using std::vector;
using std::cout;
using std::endl;

namespace StetAlgo{

template<typename EdgeWeightType>
struct Edge{
    int source;
    int dest;
    EdgeWeightType weight;
    Edge(int _source, int _dest, EdgeWeightType _w)
        : source(_source), dest(_dest), weight(_w)
    {}
};

template<typename EdgeWeightType>
std::ostream& operator<<(std::ostream& oo, const Edge<EdgeWeightType>& e){
    oo << e.source << ' '<<e.dest << ' '<<e.weight ; return oo;
}

template<typename EdgeWeightType>
class Graph{
    public:
        using EE = Edge<EdgeWeightType>;
    struct MinimalEdge{
        int dest;
        EdgeWeightType weight;
        MinimalEdge(int _dest, EdgeWeightType _w)
            : dest(_dest), weight(_w)
        {}
    };
private:
    vector<vector<MinimalEdge> > con;
    int edgeCount;
    vector<int> backtrack;
    vector<EdgeWeightType> cost;
    int distSource=-1;
public:
    Graph(int n):
        con(n, vector<MinimalEdge>()), edgeCount{0}
    {}

    void addEdge(std::pair<int,int> both, EdgeWeightType weight, bool twoway=true){
        if(twoway) con[both.second].push_back({both.first,weight});
        con[both.first].push_back({both.second,weight});
        edgeCount++;
    }
    inline int V() const{
        return con.size();
    }
    inline int E() const{
        return edgeCount;
    }
    vector<EE> kruskal() const{
        vector<EE> ret;
        auto crit = [](const EE& e1,const EE& e2)->bool {return e1.weight>e2.weight;};
        std::priority_queue<EE,vector<EE>, decltype(crit)> PQ(crit);
        DisjointSet D( V() );
        for(int source=0;source<V();++source){
            for(auto& y: con[source]){
                if(y.dest > source)
                    PQ.push({source, y.dest, y.weight});
            }
        }

        while(!PQ.empty() && ret.size() < V()-1){
            EE e = PQ.top(); PQ.pop();
            if( ! D.connected(e.source,e.dest)) {
                ret.push_back(e);
                D.connect(e.source,e.dest);
            }
        }

        if(ret.size() < V()-1) return vector<EE>();
        else return ret;
    }

    void dijkstra(int source){
        distSource = source;
        auto crit = [](const EE& e1,const EE& e2)->bool {return e1.weight>e2.weight;};
        std::priority_queue<EE,vector<EE>,decltype(crit)> PQ(crit);
        backtrack = vector<int>(V(),-1);
        cost = vector<EdgeWeightType>(V(),LLONG_MAX);
        backtrack[source] = source; 
        cost[source] = 0;
        for(auto& edge: con[source]){
            PQ.push({source, edge.dest, edge.weight});
        }
        while(!PQ.empty()){
            EE now = PQ.top(); PQ.pop();
            if( backtrack[now.dest] != -1 ) continue;
            backtrack[now.dest] = now.source;
            cost[now.dest] = now.weight;
            for(auto& edge: con[now.dest]){
                if( backtrack[edge.dest] != -1 ) continue;
                PQ.push({now.dest,edge.dest,cost[now.dest] + edge.weight});
            }
        }
    }
    vector<EE> path(int s,int e){
        vector<EE> ret;
        if(backtrack[e]==-1) return vector<EE>();
        int now = e;
        while(backtrack[now] != now){
            ret.push_back({backtrack[now], now, cost[now]-cost[backtrack[now]]});
            now = backtrack[now];
        }
        std::reverse(ret.begin(),ret.end());
        return ret;
    }
    bool connected(int s,int e){
        return backtrack[e] != -1;
    }
    EdgeWeightType distance(int s,int e){
        return cost[e];
    }

    
};


}
#endif
