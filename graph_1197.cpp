#include"graph.hpp"
#include<numeric>
using namespace StetAlgo;
using namespace std;

int main(){
    cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(false);
    int v,e;
    int a,b,c;
    cin >> v >> e;
    Graph<long long int> G(v);
    while(e--){
        cin >> a>>b>>c;
        G.addEdge({a-1,b-1},c,true);
    }
    auto l = G.kruskal();
    long long int all = 0;
    for(auto& e : l){
        all += e.weight;
    }
    cout << all << '\n';
}
