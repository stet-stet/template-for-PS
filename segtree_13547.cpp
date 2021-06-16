#include"segtree.hpp"
#include"stdc++.hpp"
using namespace std;
using namespace StetAlgo;

struct query{
    int l,r,origindex,res;
    query(int _l=0,int _r=0,int _origindex=0)
        : l{_l}, r{_r}, origindex{_origindex}
    {}
};

int main(){
    cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(false);

    int n; cin >> n;
    vector<int> arr(n);
    for(int i=0;i<n;++i) cin >> arr[i];

    int Q; cin >> Q;
    vector<query> queries(Q);
    for(int i=0;i<Q;++i){
        cin >> queries[i].l >> queries[i].r;
        queries[i].origindex = i;
    }
    sort(queries.begin(),queries.end(),[](query x,query y){return x.r<y.r;});

    auto tree = makeSegTree<100000>( vector<int>(), [](int x,int y){return x+y;} );
    int tree_inch=1;
    unordered_map<int,int> whereWasThis;
    
    for(int i=0;i<queries.size();){
        while(tree_inch <= queries[i].r){
            tree[ tree_inch-1 ] = 1;
            if(whereWasThis.count(arr[tree_inch-1]) > 0){
                tree[ whereWasThis[ arr[tree_inch-1] ] ] = 0;
            }
            whereWasThis[ arr[tree_inch-1] ] = tree_inch-1;
            ++tree_inch;
        }
        while( i<queries.size() && tree_inch > queries[i].r ){
            queries[i].res = tree.reduce({queries[i].l-1,queries[i].r});
            ++i;
        }
    }

    sort(queries.begin(),queries.end(),[](query x,query y){return x.origindex<y.origindex;});
    for(int i=0;i<Q;++i){
        cout << queries[i].res << '\n';
    }
}
