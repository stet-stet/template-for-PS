#include"millerrabin.hpp"
#include<iostream>
#include<algorithm>
#include<vector>
using namespace StetAlgo;
using namespace std;

int main(){
//    cout.tie(NULL); cin.tie(NULL);     ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    vector<unsigned long long> V(n);
    for(int i=0;i<n;++i){
        cin >> V[i];
        V[i] = 2*V[i] + 1;
    }
    
    int count=0;
    for(int i=0;i<n;++i){
        if(millerRabin(V[i],30)) ++count;
    }
    cout << count;
}
