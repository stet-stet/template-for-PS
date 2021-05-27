#include"stetsegtree.hpp"
using namespace StetAlgo;
using namespace std;
int main(){
    auto reducer = [](int a,int b){return a+b;};
    long long n,m;
    cin >> n >> m ;
    vector<long long int> a(n);
    for(int i=0;i<n;i++) cin >> a[i];
    auto tree = makeSegTree<100000>(a,reducer,0ll);
    for(int i=0;i<m;++i){
        int l,r;
        cin>>l>>r;
        cout << tree.reduce({l-1,r-1})<<endl;
    }
}
