#include"commonpatterns.hpp"
#include<iostream>
using namespace std;
using namespace StetAlgo;
int main(){
    cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false);

    auto PQ = makeMinHeap<int>();
    int n,x; 
    cin>>n;
    while(n--){
        cin>>x;
        if(x>0) PQ.push(x);
        else{
            if(PQ.size()){ cout << PQ.top() <<"\n"; PQ.pop();}
            else cout << "0\n";
        }
    }
}
