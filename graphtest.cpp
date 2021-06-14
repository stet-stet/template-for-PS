#include"graph.hpp"
using namespace StetAlgo;
using namespace std;

int main(){
    Graph<int> G(8);
    G.addEdge({1,7},12);
    G.addEdge({4,7},13);
    G.addEdge({1,4},18);
    G.addEdge({1,2},67);
    G.addEdge({1,5},17);
    G.addEdge({2,4},24);
    G.addEdge({2,5},62);
    G.addEdge({3,5},20);
    G.addEdge({3,6},7);
    G.addEdge({5,6},45);
    G.addEdge({5,7},73);

    auto k = G.dijkstra(1,6);
    for(auto& x: k){
        cout << x <<endl;
    }
}
