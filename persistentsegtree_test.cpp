#include"persistentsegtree.hpp"
#include<numeric>
using namespace std;
using namespace StetAlgo;
int main(){
    vector<long long> V (100000);
    std::iota(V.begin(),V.end(),0);
    auto reducer = [](int a,int b){return a+b;};
    int initChk = 0;
    auto tree = makePersistentSegTree<100000>(V,reducer,initChk,0ll);
    cout << tree.reduce({1,1000},initChk)<<endl;
    tree.update(1,9);
    tree.update(1000,1001);
    cout << tree.reduce({1,1000},initChk)<<endl;
}
