#include"disjointset.hpp"
#include<iostream>

using namespace StetAlgo;
using namespace std;

int main(){
    cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false);
    int n; cin >> n;
    DisjointSet D(n);
    int m; cin >> m;
    while(m--){
        int query, a, b;
        cin >> query>>a>>b;
        if(query==0) D.connect(a,b);
        else cout << ( D.connected(a,b) ? "YES" : "NO" ) << '\n';
    }
}
