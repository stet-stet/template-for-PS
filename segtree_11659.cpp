#include"segtree.hpp"
#include<cstdio>
#include<iostream>
using namespace StetAlgo;
using namespace std;
int main(){
    cin.tie(NULL); cout.tie(NULL); sync_with_stdio(false);
    auto reducer = [](int a,int b){return a+b;};
    long long n,m;
    scanf("%lld%lld",&n,&m);
    vector<long long int> a(n);
    for(int i=0;i<n;i++) scanf("%lld",&a[i]);
    auto tree = makeSegTree<100000>(a,reducer,0ll);
    for(int i=0;i<m;++i){
        int l,r;
        scanf("%d%d",&l,&r);
        printf("%lld\n",tree.reduce({l-1,r-1}));
    }
}
