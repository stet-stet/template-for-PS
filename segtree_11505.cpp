#include"segtree.hpp"
#include<iostream>
using namespace StetAlgo;
using namespace std;
int main(){
    cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false);
    auto reducer = [](long long a,long long b){return (a*b)%1000000007ll;};
    int n,m,k;
    cin >> n >> m >> k;
    vector<long long> a (n);
    for(size_t i=0;i<n;++i){
        cin >> a[i];
    }
    auto tree = makeSegTree<1000000>(a,reducer);
    for(int i=0;i<m+k;++i){
        int a;
        size_t b;
        long long int c;
        cin >> a>>b>>c;
        if(a==1) tree[b-1] = c;
        else{
            cout << tree.reduce({b-1,c-1}) << '\n';
        }
    }
}
