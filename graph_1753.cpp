#include"graph.hpp"
#include<iostream>
using namespace std;
using namespace StetAlgo;


int main(){
    cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false);
    int v,e, start, source, dest, weight;
    cin >> v >> e >> start;
    Graph<long long> G (v+1);
    for(int i=0;i<e;++i){
        cin >> source>>dest>>weight;
        G.addEdge({source,dest},weight,false);//not both ways.
    }
    G.dijkstra(start);
    for(int i=1;i<=v;++i){
        if(G.connected(start,i)) cout << G.distance(start,i) <<'\n';
        else cout << "INF" << '\n';
    }
}
