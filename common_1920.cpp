#include"commonpatterns.hpp"
#include<iostream>
#include<algorithm>
using namespace StetAlgo;
using namespace std;
int main(){
    cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    vector<int> V(n);
    while(n--){
        cin >> V[n];
    }
    sort(V.begin(),V.end());
    int m;
    cin >> m;
    while(m--){
        int x;
        cin >> x;
        cout << binary_search(V,x) << '\n';
    }
}
