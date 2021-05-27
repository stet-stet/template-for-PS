#include"segtree.hpp"
#include<iostream>
using namespace StetAlgo;
using namespace std;
int main(){
    cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false);
    auto reducer = [](__int128 a,__int128 b){return a+b;};
    int n,m,k;
    cin >> n >> m >> k;
    vector<__int128> a (n);
    for(size_t i=0;i<n;++i){
	long long aaa;
        cin >> aaa;
	a[i] = aaa;
    }
    __int128 zero=0ll;
    auto tree = makeSegTree<1000000>(a,reducer,zero);
    for(int i=0;i<m+k;++i){
        int a;
        size_t b;
        long long int c;
        cin >> a>>b>>c;
        if(a==1) tree[b-1] = c;
        else{
	    __int128 ans = tree.reduce({b-1,c-1});
	    
            cout << (long long int) ans << '\n';
        }
    }
}
