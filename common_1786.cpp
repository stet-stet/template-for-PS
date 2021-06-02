#include"commonpatterns.hpp"
#include<iostream>
using namespace StetAlgo;
using namespace std;
int main(){
    cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false);
    string up,down;
    getline(cin, up);
    getline(cin, down);
//    cout << up << down << endl;
        
    auto ret = KMPSearch(up,down);
    cout << ret.size() << '\n';
    for(auto x: ret) cout << x+1 << '\n';
}
