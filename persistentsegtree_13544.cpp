#include"persistentsegtree_faster.hpp"
#include"stdc++.hpp"
using namespace std;
using namespace StetAlgo;
int main(){
    cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(false);

    int initChk = 0;
    int (*reducer)(int,int) = [](int x,int y){return x+y;};
    auto tree = makePersistentSegTree<100000>(vector<int>(), reducer, initChk);

    int n;
    cin >> n;
    vector<int> arr(n);
    for(int i=0;i<n;++i) cin >> arr[i];
    vector<int> tosort = arr;
    sort(tosort.begin(),tosort.end(),[](int a,int b){return a<b;});

    unordered_map<int,int> coord;
    int nowcoord = 0;
    coord[tosort[0]] = nowcoord;
    for(int i=1;i<n;++i){
        if(tosort[i] > tosort[i-1]) ++nowcoord;
        coord[tosort[i]] = nowcoord;
    }

    // into the tree.
    for(int i=1;i<=n;++i){
        int where = coord[arr[i-1]];
        tree.checkout(i, false);
        tree.update(where, tree.get(where,i) + 1);
    }

    int Q;
    cin >> Q;

    int ans=0;
    for(int i=0;i<Q;++i){
        int l,r,k;
        cin >>l>>r>>k;
        l ^= ans;
        r ^= ans;
        k ^= ans;
        auto itt = lower_bound(tosort.begin(),tosort.end(),k+1);
        if(itt != tosort.end()){
            ans = tree.reduce(make_pair(coord[(*itt)],100003), r);
            ans -= tree.reduce(make_pair(coord[(*itt)],100003), l-1);
        } else {
            ans = 0;
        }
        cout << ans << '\n';
    }
    
}
